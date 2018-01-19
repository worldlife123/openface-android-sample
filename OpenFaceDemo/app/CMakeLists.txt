cmake_minimum_required(VERSION 3.4.1)

# This sets the C++ standard to c++14 and required for all the following targets that we define.
# It has no effect on MSVC though - we thus define more specific requirements for each executable target respectively.
# Also it will not apply to the eos library target, since it is an INTERFACE_LIBRARY, and these properties do not apply to interface libraries.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # This makes CMake use -std=c++11 instead of -std=gnu++11

SET(OPENFACE_DIR ../../Openface_reduced/lib/local/LandmarkDetector)
SET(NATIVE_CODE_DIR ../../ios/CaptureAndRender/NativeCode)
SET(OpenCV_FOUND true )
FIND_PACKAGE(OpenCV REQUIRED)
MESSAGE(STATUS "OpenCV found at ${OpenCV_INCLUDE_DIRS}")
#Fix opencv library paths
set(libs "${OpenCV_DIR}/../libs")

add_library(libopencv_java3 SHARED IMPORTED )
set_target_properties(libopencv_java3 PROPERTIES
    IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libopencv_java3.so")

#add_library(libopencv_java SHARED IMPORTED )
#set_target_properties(libopencv_java PROPERTIES
#    IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libopencv_java.so")

#add_library(libopencv_calib3d STATIC IMPORTED )
#set_target_properties(libopencv_calib3d PROPERTIES
#    IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libopencv_calib3d.a")
#
#add_library(libopencv_core STATIC IMPORTED )
#set_target_properties(libopencv_core PROPERTIES
#    IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libopencv_core.a")
#
#add_library(libopencv_dnn STATIC IMPORTED )
#set_target_properties(libopencv_core PROPERTIES
#    IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libopencv_dnn.a")
#
#add_library(libopencv_features2d STATIC IMPORTED )
#set_target_properties(libopencv_features2d PROPERTIES
#    IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libopencv_features2d.a")
#
#add_library(libopencv_flann STATIC IMPORTED )
#set_target_properties(libopencv_flann PROPERTIES
#    IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libopencv_flann.a")
#
#add_library(libopencv_highgui STATIC IMPORTED )
#set_target_properties(libopencv_highgui PROPERTIES
#    IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libopencv_highgui.a")
#
#add_library(libopencv_imgcodecs STATIC IMPORTED )
#set_target_properties(libopencv_imgcodecs PROPERTIES
#    IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libopencv_imgcodecs.a")
#
#add_library(libopencv_imgproc STATIC IMPORTED )
#set_target_properties(libopencv_imgproc PROPERTIES
#    IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libopencv_imgproc.a")
#
#add_library(libopencv_ml STATIC IMPORTED )
#set_target_properties(libopencv_ml PROPERTIES
#    IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libopencv_ml.a")
#
#add_library(libopencv_objdetect STATIC IMPORTED )
#set_target_properties(libopencv_objdetect PROPERTIES
#    IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libopencv_objdetect.a")
#
#add_library(libopencv_photo STATIC IMPORTED )
#set_target_properties(libopencv_photo PROPERTIES
#    IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libopencv_photo.a")
#
#add_library(libopencv_shape STATIC IMPORTED )
#set_target_properties(libopencv_shape PROPERTIES
#    IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libopencv_shape.a")
#
#add_library(libopencv_stitching STATIC IMPORTED )
#set_target_properties(libopencv_stitching PROPERTIES
#    IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libopencv_stitching.a")
#
#add_library(libopencv_superres STATIC IMPORTED )
#set_target_properties(libopencv_superres PROPERTIES
#    IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libopencv_superres.a")
#
#add_library(libopencv_video STATIC IMPORTED )
#set_target_properties(libopencv_video PROPERTIES
#    IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libopencv_video.a")
#
#add_library(libopencv_videoio STATIC IMPORTED )
#set_target_properties(libopencv_videoio PROPERTIES
#    IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libopencv_videoio.a")
#
#add_library(libopencv_videostab STATIC IMPORTED )
#set_target_properties(libopencv_videostab PROPERTIES
#    IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libopencv_videostab.a")
SET(OpenCV_LIBS
    libopencv_java3 #used for java sdk
    #libopencv_java #same to static libs
    #17 static libs in total
    #libopencv_calib3d
    #libopencv_core
    #libopencv_dnn
    #libopencv_features2d
    #libopencv_flann
    #libopencv_highgui
    #libopencv_imgcodecs
    #libopencv_imgproc
    #libopencv_ml
    #libopencv_objdetect
    #libopencv_photo
    #libopencv_shape
    #libopencv_stitching
    #libopencv_superres
    #libopencv_video
    #libopencv_videoio
    #libopencv_videostab
   )
MESSAGE(STATUS "OpenCV libs found at ${OpenCV_LIBS}")
ADD_SUBDIRECTORY(${dlib_DIR} ${CMAKE_CURRENT_BINARY_DIR}/dlib)
# Load openface sources
FILE(GLOB OPENFACE_SOURCES
    "${OPENFACE_DIR}/src/*.cpp"
 )
MESSAGE("source ${OPENFACE_SOURCES} ")
SET(GLOB OPENFACE_HEADERS
     "${OPENFACE_DIR}/include/*.h"
    )
# Load native code sources
FILE(GLOB NATIVECODE_SOURCES
    "${NATIVE_CODE_DIR}/*.h"
    "${NATIVE_CODE_DIR}/*.cpp"
)

add_library( # Sets the name of the library.
             native-lib
             SHARED
            ${OPENFACE_SOURCES}
            ${OPENFACE_HEADERS}
            ${NATIVECODE_SOURCES}
            src/main/cpp/native-lib.cpp )

find_library( # Sets the name of the path variable.
              log-lib
              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

INCLUDE_DIRECTORIES(src/main/cpp/
                  ${OpenCV_INCLUDE_DIRS}
                  ${NATIVE_CODE_DIR}
                  ${OPENFACE_DIR}/include
                 )

target_link_libraries( native-lib
                       ${log-lib}
                       dlib
                       ${OpenCV_LIBS}
                       )