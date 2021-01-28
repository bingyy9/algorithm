package com.example.testjni;

import android.app.Application;

public class BaseApplication extends Application {

    @Override
    public void onCreate() {
        super.onCreate();
        SignatureUtils.signatureVerify(this);
    }
}
