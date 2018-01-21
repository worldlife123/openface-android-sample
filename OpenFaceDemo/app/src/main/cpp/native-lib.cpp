#include <jni.h>
#include "native-lib.h"
#include "fps.h"
#include <cmath>
#include <opencv2/opencv.hpp>
#include <sstream>

#include "NativeCode.h"

#include <android/log.h>

#define  LOG_TAG    "OpenFaceJNI"

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#include <iostream>
#include <streambuf>

class MyStreamBuf : public std::streambuf
{
    enum
    {
        BUFFER_SIZE = 255,
    };

public:
    MyStreamBuf()
    {
        buffer_[BUFFER_SIZE] = '\0';
        setp(buffer_, buffer_ + BUFFER_SIZE - 1);
    }

    ~MyStreamBuf()
    {
        sync();
    }

protected:
    virtual int_type overflow(int_type c)
    {
        if (c != EOF)
        {
            *pptr() = c;
            pbump(1);
        }
        flush_buffer();
        return c;
    }

    virtual int sync()
    {
        flush_buffer();
        return 0;
    }

private:
    int flush_buffer()
    {
        int len = int(pptr() - pbase());
        if (len <= 0)
            return 0;

        if (len <= BUFFER_SIZE)
            buffer_[len] = '\0';

#ifdef ANDROID
        android_LogPriority t = ANDROID_LOG_INFO;
        __android_log_write(t, "native-cout", buffer_);
#else
        printf("%s", buffer_);
#endif

        pbump(-len);
        return len;
    }

private:
    char buffer_[BUFFER_SIZE + 1];
};

MyStreamBuf g_MyStreamBuf;

Native* m_nativeCode = nullptr;
bool m_isInitFinished = false;

JNIEXPORT void JNICALL
Java_org_utils_JniManager_init(JNIEnv *env, jclass type, jstring resourceDir_,
                               jint inputWidth, jint inputHeight,
                               jint neuralNetType) {
    std::cout.rdbuf(&g_MyStreamBuf);//redirect std::cout
    if (!m_isInitFinished) {
        LOGI("Start initialization process");
        const char *resourceDir = env->GetStringUTFChars(resourceDir_, 0);

        // TODO
        std::string resourceDirStr = std::string(resourceDir);
        m_nativeCode = new Native(resourceDirStr);
        m_nativeCode->init(std::string(), std::string());
        LOGI("Finished init call!");

//    m_clnf_model = LandmarkDetector::CLNF(std::string(deepNetFilename) + "/main_clnf_ibug_glasses_movile.txt");
//    m_det_parameters.model_location = std::string(deepNetFilename) + "/main_clnf_ibug_glasses_movile.txt";
//    m_det_parameters.multi_view = false;
//    m_det_parameters.track_gaze = false;
//    m_det_parameters.validate_detections = false;
//    mFps = new Fps();

        m_isInitFinished = true;
        env->ReleaseStringUTFChars(resourceDir_, resourceDir);
    }
}

JNIEXPORT void JNICALL
Java_org_utils_JniManager_process(JNIEnv *env, jclass type, jlong colorImage, jlong greyImage) {
    cv::Mat &colorImg = *(cv::Mat *) colorImage;
    cv::Mat outResult;

    if (m_isInitFinished) m_nativeCode->processFrame(colorImg);
}

JNIEXPORT void JNICALL
Java_org_utils_JniManager_start(JNIEnv *env, jclass type) {

}

JNIEXPORT void JNICALL
Java_org_utils_JniManager_stop(JNIEnv *env, jclass type) {

    // TODO

}