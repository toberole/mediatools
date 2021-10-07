#include "java_MediaTools.h"
#include "log.h"
#include <string>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/opt.h"
#include "libavutil/imgutils.h"
}


/*
 * Class:     com_zw_mediatools_MediaTools
 * Method:    native_exec
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_zw_mediatools_MediaTools_native_1exec
        (JNIEnv *env, jclass jclazz, jstring jStr_cmd) {

    return 0;
}

/*
 * Class:     com_zw_mediatools_MediaTools
 * Method:    play
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL
Java_com_zw_mediatools_MediaTools_play(JNIEnv *env, jclass jclazz, jstring jstr_url) {
    LOGI("play ......");

    AVCodec *avCodec = nullptr;
    const char *str_url = env->GetStringUTFChars(jstr_url, 0);
    int len = env->GetStringUTFLength(jstr_url);
    std::string url(str_url, len);

    AVFormatContext *avFormatContext = avformat_alloc_context();
    int ret = avformat_open_input(&avFormatContext, url.c_str(), NULL, NULL);
    if (ret != 0) {
        LOGE("avformat_open_input error ret: %d", ret);
        return;
    }

    ret = avformat_find_stream_info(avFormatContext, NULL);
    if (ret < 0) {
        LOGE("avformat_find_stream_info error ret: %d", ret);
        return;
    }

    int videoIndex = -1;
    int audioIndex = -1;
    for (int i = 0; i < avFormatContext->nb_streams; i++) {
        if (avFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoIndex = i;
        } else if (avFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioIndex = i;
        }
    }

    if (videoIndex == -1) {
        LOGE("Couldn't find a video stream.");
        return;
    }

    if (audioIndex == -1) {
        LOGE("Couldn't find a audio stream.");
        return;
    }

    LOGI("videoIndex: %d,audioIndex: %d", videoIndex, audioIndex);

    env->ReleaseStringUTFChars(jstr_url, str_url);
}

