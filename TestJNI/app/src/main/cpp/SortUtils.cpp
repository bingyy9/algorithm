//
// Created by bingy on 2021/2/4.
//

#include "SortUtils.h"
#include <stdlib.h>
#include <ctime>
#include <assert.h>
#include <cstring>

#include <android/log.h>
#define TAG "JNI_TAG"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

#define TAG "TAG"
#define printT(number) print("TAG", number)

void print(char* tag, int number){

}

void SortUtils::bubbleSort(int* arr, int len){
   for(int i = 0; i < len - 1; i++){
       bool swap = false;
//       LOGE("i=%d", i);
       for(int j = 0; j < len - 1 -i; j++){
//           LOGE("j=%d", j);
           if(arr[j] > arr[j+1]){
               std::swap(arr[j], arr[j+1]);
//               int tmp = arr[j];
//               arr[j] = arr[j+1];
//               arr[j+1] = tmp;
               swap = true;
           }
       }
       if(!swap){
           break;
       }
   }

//   for(int i = 0; i<len; i++){
////       LOGE("%d", arr[i]);
//   }
}

void SortUtils::optimize_bubbleSort(int* arr, int len){
    for(int i = 0; i < len - 1; i++){
        bool swap = false;
//       LOGE("i=%d", i);
        for(int j = 0; j < len - 1 -i; j++){
//           LOGE("j=%d", j);
            if(arr[j] > arr[j+1]){
                std::swap(arr[j], arr[j+1]);
//               int tmp = arr[j];
//               arr[j] = arr[j+1];
//               arr[j+1] = tmp;
                swap = true;
            }
        }
        if(!swap){
            break;
        }
    }

//   for(int i = 0; i<len; i++){
////       LOGE("%d", arr[i]);
//   }
}

void SortUtils::insert_sort(int* arr, int len){
    for(int i = 1; i<len; i++){
        for(int j = i; j > 0; j--){
            if(arr[j] < arr[j-1]){
                std::swap(arr[j], arr[j-1]);
            } else {
                break;
            }
        }
    }
}

void SortUtils::insert_sort_optimize(int* arr, int len){
    int j;
    for(int i = 1; i<len; i++){
        int tmp = arr[i];
        for(j = i; j > 0; j--){
            if(tmp < arr[j-1]){
                arr[j] = arr[j-1];
//                std::swap(arr[j], arr[j-1]);
            } else {
                break;
            }
        }
        arr[j] = tmp;
    }
}

void SortUtils::hill_sort(int* arr, int len){
    int step = len/2;

    while(step > 0) {
        for (int k = 0; k < step; k++) { //分成step个组，每组按照插入排序算法
            for (int i = k+step; i < len; i += step) {
                for (int j = i; j > k; j -= step) {
                    if (arr[j] < arr[j - step]) {
                        std::swap(arr[j], arr[j - step]);
                    } else {
                        break;
                    }
                }
            }
        }
        step/=2;
    }
}

//对数组[lo, mid] and [mid+1, hi]进行归并
void merge_(int *arr, int lo, int mid, int hi) {
    //1. 对数组进行一次拷贝
    int tmp[hi - lo + 1];
    for(int i=lo; i<=hi; i++){
        tmp[i-lo] = arr[i];
    }

    int i = lo;
    int j = mid + 1;
    int k = lo;
    for(; k <= hi; k++){
        if(i > mid){
            arr[k] = tmp[j - lo];
            j++;
        } else if(j > hi){
            arr[k] = tmp[i - lo];
            i++;
        }
        //临时数据里面的i和j的位置
        else if(tmp[i - lo] < tmp[j - lo]){
            arr[k] = tmp[i - lo];
            i++;
        } else {
            arr[k] = tmp[j - lo];
            j++;
        }
    }
}

//对数组的[lo, hi]闭区间进行归并排序
void mergeSort_(int* arr, int lo, int hi){
    //递归到底的情况
    if(lo >= hi){
        return;
    }

    int mid = (lo + hi) >> 1;
    mergeSort_(arr, lo, mid);
    mergeSort_(arr, mid + 1, hi);
    if(arr[mid] > arr[mid + 1]){ //优化
        merge_(arr, lo, mid, hi);
    }
}

void SortUtils::merge_sort(int* arr, int len){
    mergeSort_(arr, 0, len-1);
}

//对数组arr区间p
int partition(int* arr, int left, int right){
    //优化，跟区间[left, right]随机位置进行比较
    std::swap(arr[left], arr[rand() % (right - left + 1)]);


    int key = arr[left];
    //以p为分割， [left + 1, p] < key < [p + 1, right]
    int p = left;
    for(int i=left; i<=right; i++){
        if(arr[i] < key){
            std::swap(arr[p+1], arr[i]);
            p++;
        }
    }

    std::swap(arr[left], arr[p]);
    return p;
}

void quick_sort_(int* arr, int left, int right){
    if(left >= right){
        return;
    }

    int p = partition(arr, left, right);
    quick_sort_(arr, left, p - 1);
    quick_sort_(arr, p+1, right);

}

void SortUtils::quick_sort(int* arr, int len){
    srand(time(NULL));
    quick_sort_(arr, 0, len-1);
}

int* SortUtils::create_random_array(int len, int low, int high){
    int *arr = new int[len];
    for(int i=0; i<len; i++){
        arr[i] = rand()%(high - low) + low;
    }
    return arr;
}

void SortUtils::selectSort(int* arr, int len){
    for(int i = 0; i < len - 1; i++){
        int min = i;
//        LOGE("i=%d", i);
        for(int j = i+1; j < len; j++){
//            LOGE("j=%d", j);
            if(arr[min] > arr[j]){
                min = j;
            }
        }
        if(min != i){
            std::swap(arr[min], arr[i]);
//            int tmp = arr[min];
//            arr[min] = arr[i];
//            arr[i] = tmp;
        }
    }

    for(int i = 0; i<len; i++){
//        LOGE("%d", arr[i]);
    }
}

void SortUtils::sort_array(char* sortName, void(*sort)(int*, int), int *arr, int len)
{
    clock_t start_time = std::clock();
    sort(arr, len);
    clock_t end_time = std::clock();

    LOGE("%s time: %lf", sortName, (double)(end_time-start_time)/CLOCKS_PER_SEC);

//    print_array(arr, len);
    for(int i = 0; i<len - 1; i++){
//        assert(arr[i] <= arr[i+1]);
    }
}

int* SortUtils::copy_rand_array(int* arr, int len){
    int* copy_arr = new int[len];
    memcpy(copy_arr, arr, sizeof(int)*len);
    return copy_arr;
}

void SortUtils::print_array(int* arr, int len){
    for(int i = 0; i<len; i++){
        LOGE("%d, ", arr[i]);
    }
}

void shiftDown(int* arr, int i, int n) {
    while(i*2 + 1 < n){
        int max = 2*i + 1;

        //有右孩子且大于左孩子
        if(max + 1 < n && arr[max+1] > arr[max]){
            max = max + 1;
        }
        if(arr[i] > arr[max]){
            break;
        }
        std::swap(arr[i], arr[max]);
        i = max;
    }
}

void SortUtils::heap_sort(int* arr, int len){
    //1.构建大顶堆
    for(int i = len/2 - 1; i>=0; --i){
        //从第一个非叶子结点从下至上，从右至左调整结构
        shiftDown(arr, i, len);
    }

    LOGE("///////");
    SortUtils::print_array(arr, 5);
    LOGE("///////");
    //2.调整堆结构+交换堆顶元素与末尾元素
    for(int i=len-1; i>0; i--){
        std::swap(arr[0], arr[i]);//将堆顶元素与末尾元素进行交换, 将堆顶元素与末尾元素交换，将最大元素"沉"到数组末端;
        shiftDown(arr, 0, i);//重新对堆进行调整
    }
}