cmake_minimum_required(VERSION 3.10.2)

project("mediatools")

include_directories(${CMAKE_SOURCE_DIR}/third/ffmpeg/${CMAKE_ANDROID_ARCH_ABI})
include_directories(${CMAKE_SOURCE_DIR}/include)

aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC_FILES)

link_directories(${CMAKE_SOURCE_DIR}/third/libs/${CMAKE_ANDROID_ARCH_ABI})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=format-security")

add_library(mediatools-lib SHARED ${SRC_FILES})

target_link_libraries(
        mediatools-lib
        -Wl,--start-group# 忽略库的链接顺序
        # ffmpeg avcodec avfilter avformat avutil swresample swscale
        avcodec
        avfilter
        avformat
        avutil
        swresample
        swscale
        avdevice
        postproc
        -Wl,--end-group# 忽略库的链接顺序
        # android
        log
        z
        android
        OpenSLES
        GLESv2
        EGL
)