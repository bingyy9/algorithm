//
// Created by bingy on 2021/2/3.
//

#ifndef TESTJNI_CARDOCR_H
#define TESTJNI_CARDOCR_H

#include <jni.h>
#include <android/bitmap.h>
#include "opencv2/opencv.hpp"

using namespace cv;

namespace co1{
    //0 means found
    int find_card_area(const Mat& mat, Rect& area);

    int find_card_number_area(const Mat& mat, Rect& area);

    int find_card_numbers(const Mat& mat, std::vector<Mat>& numbers);
};

namespace co2{

}

#endif //TESTJNI_CARDOCR_H
