package com.zw.mediatools;

import android.app.Application;

import com.tencent.bugly.crashreport.CrashReport;

public class App extends Application {
    @Override
    public void onCreate() {
        super.onCreate();
        CrashReport.initCrashReport(this, "02ef267e11", true);
    }
}