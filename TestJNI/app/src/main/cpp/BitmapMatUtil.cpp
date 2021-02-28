//
// Created by bingy on 2021/2/3.
//

#include "BitmapMatUtil.h"

int BitmapMatUtil::mat2bitmap(JNIEnv* env, jobject &bitmap, Mat &mat){
    AndroidBitmapInfo  info;
    void *pixels;
    AndroidBitmap_getInfo(env, bitmap, &info);

    AndroidBitmap_lockPixels(env, bitmap, &pixels);
    if(info.format == ANDROID_BITMAP_FORMAT_RGBA_8888){
        Mat temp(info.height, info.width, CV_8UC4, pixels);
        if(mat.type() == CV_8UC4){
            mat.copyTo(temp);
        } else if(mat.type() == CV_8UC2){
            cvtColor(mat, temp, COLOR_BGR5652BGRA);
        } else if(mat.type() == CV_8UC1){
            cvtColor(mat, temp, COLOR_GRAY2RGBA);
        }
    } else if(info.format == ANDROID_BITMAP_FORMAT_RGB_565){
        Mat temp(info.height, info.width, CV_8UC2, pixels);
        if(mat.type() == CV_8UC2){
            mat.copyTo(temp);
        } else if(mat.type() == CV_8UC4){
            cvtColor(mat, temp, COLOR_BGRA2RGB);
        } else if(mat.type() == CV_8UC1){
            cvtColor(mat, temp, COLOR_GRAY2BGR565);
        }
    }
    AndroidBitmap_unlockPixels(env, bitmap);
    return 0;
}

int BitmapMatUtil::bitmap2mat(JNIEnv* env, jobject bitmap, Mat &mat){
//Mat 里面有个type：CV_8UC4--ARGB8888  CV_8UC2-RGB565
    AndroidBitmapInfo info;
    AndroidBitmap_getInfo(env, bitmap, &info);

    //锁定Bitmap画布
    void* pixels;
    AndroidBitmap_lockPixels(env, bitmap, &pixels);

    //指定mat的宽高和type BGRA
    Mat createMat(info.height, info.width, CV_8UC4);
//    mat.create(info.height, info.width, CV_8UC4);
    if(info.format == ANDROID_BITMAP_FORMAT_RGBA_8888){
        //CV_8UC4
        Mat temp(info.height, info.width, CV_8UC4, pixels);
        temp.copyTo(createMat);
//        cvtColor(temp, createMat, COLOR_BGRA2BGR565);
//        mat.create(info.height, info.width, CV_8UC4);
//        temp.copyTo(mat);
    } else if(info.format == ANDROID_BITMAP_FORMAT_RGB_565){
        Mat temp(info.height, info.width, CV_8UC2, pixels);
        cvtColor(temp, createMat, COLOR_BGR5652BGRA);
//        temp.copyTo(createMat);
    } else {
        //others need to convert bitmap firstly
    }

    createMat.copyTo(mat);
    AndroidBitmap_unlockPixels(env, bitmap);

    return 0;
}