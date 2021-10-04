package com.zw.mediatools;

public class MediaTools {
    public native static int native_exec(String cmd);

    static {
        System.loadLibrary("mediatools-lib");
    }
}
