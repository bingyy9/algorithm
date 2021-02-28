package com.example.testjni;

import android.graphics.Bitmap;

public class FaceDetection {

    //检测 保存人脸信息
    public native int faceDectionSaveInfo(Bitmap mFaceBitmap);

    public native void loadCascade(String filePath);
}
