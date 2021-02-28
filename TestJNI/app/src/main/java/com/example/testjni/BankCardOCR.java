package com.example.testjni;

import android.graphics.Bitmap;

public class BankCardOCR {
    public static native String cardOcr(Bitmap bitmap);
}
