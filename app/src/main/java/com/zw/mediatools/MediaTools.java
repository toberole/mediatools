package com.zw.mediatools;

public class MediaTools {
    public native static int native_exec(String cmd);

    public native static void play(String url);

    static {
        System.loadLibrary("avcodec");
        System.loadLibrary("avfilter");
        System.loadLibrary("avformat");
        System.loadLibrary("avutil");
        System.loadLibrary("swresample");
        System.loadLibrary("swscale");
        System.loadLibrary("avdevice");
        System.loadLibrary("postproc");

        System.loadLibrary("mediatools-lib");
    }
}
