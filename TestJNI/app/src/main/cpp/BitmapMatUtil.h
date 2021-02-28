//
// Created by bingy on 2021/2/3.
//

#ifndef TESTJNI_BITMAPMATUTIL_H
#define TESTJNI_BITMAPMATUTIL_H

#include <jni.h>
#include <android/bitmap.h>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class BitmapMatUtil{
public:
    static int mat2bitmap(JNIEnv* env, jobject &bitmap, Mat &mat);
    static int bitmap2mat(JNIEnv* env, jobject bitmap, Mat &mat);
};

#endif //TESTJNI_BITMAPMATUTIL_H
