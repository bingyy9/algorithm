package com.example.testjni;

import android.graphics.Bitmap;

import java.util.UUID;

public class VideoCompress {
    public interface CompressCallback{
        public void onCompress(int current, int total);
    }

    public static String getUUID(){
        return UUID.randomUUID().toString();
    }

    public static native void callStaticMethod();
    public static native Point createPoint();
    public native String getSignature();
    public native void compressVideo(String[] compressCommand, CompressCallback callback);

//    static {
//        System.loadLibrary("native-lib");
//    }
}
