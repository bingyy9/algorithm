package com.example.testjni;

import android.content.Context;

public class SignatureUtils {
    private long mNativePtr = 0;

    SignatureUtils(){
        mNativePtr = nativeCreate();
    }


    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    public static native String signatureParams(String params);

    public static native void signatureVerify(Context context);

    private native long nativeCreate();
    private native void nativeWriteInt(long mNativePtr, int val);
    private native void nativeSetDataPosition(long mNativePtr, int pos);
    private native int nativeReadInt(long mNativePtr);

    public void writeInt(int i){
        nativeWriteInt(mNativePtr, i);
    }

    public void setDataPosition(int pos){
        nativeSetDataPosition(mNativePtr, pos);
    }

    public int readInt(){
        return nativeReadInt(mNativePtr);
    }


}
