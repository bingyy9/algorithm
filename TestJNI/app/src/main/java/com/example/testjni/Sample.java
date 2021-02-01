package com.example.testjni;

import java.util.UUID;

public class Sample {
    public static String name;

    public static native void sort(int[] arr);
    public static native void localRef();
    public static native void createGlobalRef(String str);
    public static native String getGlobalRef();
    public static native void delGlobalRef();

    public static native void staticLocalCache(String str);
    public static native void exception();
    public static native void arraycopy(Object src,  int  srcPos,
                                        Object dest, int destPos,
                                        int length);
}
