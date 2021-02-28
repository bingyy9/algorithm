//
// Created by bingy on 2021/2/3.
//

#include "cardocr.h"
#include <vector>
#include <android/log.h>
#define TAG "JNI_TAG"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

using namespace std;

int co1::find_card_area(const Mat& mat, Rect& area){
    //首先降噪，高斯模糊
    Mat blur;
    GaussianBlur(mat, blur, Size(5, 5), BORDER_DEFAULT, BORDER_DEFAULT);

    //边缘梯度的增强（保存图片）x,y增强
    Mat gard_x, gard_y;
    Scharr(blur, gard_x, CV_32F, 1, 0);
    Scharr(blur, gard_y, CV_32F, 0, 1);

    Mat abs_gard_x, abs_gard_y;
    convertScaleAbs(gard_x, abs_gard_x);
    convertScaleAbs(gard_y, abs_gard_y);

    //合起来
    Mat gard;
    addWeighted(abs_gard_x, 0.5, abs_gard_y, 0.5, 0, gard);

    //写到内存卡
//    imwrite("/mnt/sdcard/ocr/gard_n.jpg", gard);
//    imwrite("/storage/emulated/0/ocr/gard_n.jpg", gard);
//    imwrite("./gard_n.jpg", gard);


    Mat gray_mat;
    cvtColor(gard, gray_mat, COLOR_BGRA2GRAY);

    //二值化，
    Mat binary_mat;
    threshold(gray_mat, binary_mat, 40, 255, THRESH_BINARY);

    //找轮廓
    vector<vector<Point> > contours;
    findContours(binary_mat, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    for(int i = 0; i<contours.size(); i++){
        Rect rect = boundingRect(contours[i]);
        //过滤轮廓 宽高大于图片1/2， 不等于图片自己
        if(rect.width > mat.cols/2
            && rect.height > mat.rows/2
            && rect.width != mat.cols){
            area = rect;
            LOGE("find it");
            break;
        }
    }
    blur.release();
    gard_x.release();
    gard_y.release();
    abs_gard_x.release();
    abs_gard_y.release();
    gard.release();
    gray_mat.release();
    binary_mat.release();
//    gray_mat.copyTo(mat);
    return 0;
}

int co1::find_card_number_area(const Mat& mat, Rect& area){
    area.x = mat.cols/12;
    area.y = mat.rows/2;
    area.width = mat.cols*5/6;
    area.height = mat.rows / 4;
    return 0;
}

int co1::find_card_numbers(const Mat& mat, std::vector<Mat>& numbers){
    Mat gray_mat;
    cvtColor(mat, gray_mat, COLOR_BGRA2GRAY);
    //二值化 THRESH_OTSU 自动去找合适的值
    Mat binary;
    threshold(gray_mat, binary, 39, 255, THRESH_BINARY | THRESH_OTSU);

    //降噪过滤
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    morphologyEx(binary, binary, MORPH_CLOSE, kernel);

    //取反
    Mat binary_not = binary.clone();
    bitwise_not(binary_not, binary_not);

    //去除干扰，找数字是轮廓查询
    vector<vector<Point> > contours;
    findContours(binary_not, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    int mat_area = mat.cols * mat.rows;
    int min_height = mat.rows/4;
    int number_count = 0;
    for(int i = 0; i<contours.size(); i++){
        Rect rect = boundingRect(contours[i]);
        //去除干扰, 过滤轮廓, 去掉矮的和面积小于1/200
        int rect_area = rect.width * rect.height;
        if(rect_area < mat_area/200
            || rect.height < min_height){
            LOGE("filter this area");
            //将干扰填充颜色
            drawContours(binary, contours, i, Scalar(255), -1);
        } else {
            number_count++;
        }
    }
    LOGE("number count: %d", number_count);

    //截取数字轮廓
    binary.copyTo(binary_not);
    bitwise_not(binary_not, binary_not);

    contours.clear();
    findContours(binary_not, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    Rect rects[contours.size()];
    for(int i = 0; i<contours.size(); i++) {
        rects[i] = boundingRect(contours[i]);
    }

//    Mat contours_mat(binary, size(), CV_8UC1, Sa)

    //查找的时候顺序错乱或则粘连问题
    for(int i=0; i< contours.size() - 1 ; i++){
        for(int j=0; j<contours.size() - i -1; j++){
            if(rects[j].x > rects[j+1].x){
                swap(rects[j], rects[j+1]);
            }
        }
    }

    for(int i=0; i<contours.size(); i++){
        LOGE("%d ", rects[i].x);
//        Mat number()
    }



    gray_mat.release();
    binary.release();
    binary_not.release();
    return 0;
}