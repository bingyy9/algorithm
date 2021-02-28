//
// Created by bingy on 2021/2/25.
//

#ifndef TESTJNI_PRIORITYQUEUE_HPP
#define TESTJNI_PRIORITYQUEUE_HPP

#include <malloc.h>
using namespace std;

template <class T>
class PriorityQueue{
    int count;  //数组大小
    int index;  //当前角标
    T* array = NULL;
public:
    PriorityQueue(int count);
    ~PriorityQueue();
    bool isEmpty();
    void push(T data);
    T pop();
    void print();
private:
    void shiftUp(int index);
    void shiftDown(int i);
};

#define TAG "JNI_TAG"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
using namespace std;

template <class T>
PriorityQueue<T>::PriorityQueue(int count){
    this->count = count;
    array = new T[count];
    index = 0;
}


template <class T>
PriorityQueue<T>::~PriorityQueue(){
    if(array){
        delete[] array;
        array = NULL;
    }
    index = 0;
}

template <class T>
bool PriorityQueue<T>::isEmpty(){
    return index == 0;
}

template <class T>
void PriorityQueue<T>::push(T data){
    array[index + 1] = data;  //数据存放从1开始
    index++;

    //不断的调整堆
    shiftUp(index);
}

template <class T>
T PriorityQueue<T>::pop(){
    T value = array[1];
    array[1] = array[index];
    index--;
    shiftDown(1);
    return value;
}


template<class T>
void PriorityQueue<T>::shiftDown(int i) {
    while(i*2 <= index){ //到底的情况
        int max = 2*i; //先指向左孩子

        //有右孩子且大于左孩子
        if(max + 1 <= index && array[max+1] > array[max]){
            max = max + 1;
        }
        if(array[i] > array[max]){
            break;
        }
        swap(array[i], array[max]);
        i = max;
    }
}

template<class T>
void PriorityQueue<T>::shiftUp(int index) {
    if(index <= 1){ //0不存数据，且1/2= 0
        return;
    }

    if(array[index] > array[index/2]){
        T tmp = array[index];
        array[index] = array[index/2];
        array[index/2] = tmp;

        shiftUp(index/2);  //if里面才需要继续上浮
    }
}

template<class T>
void PriorityQueue<T>::print() {
    if(array == NULL || index == 0){
        return;
    }

    LOGE("PriorityQueue:");
    for(int i = 0; i<index; i++){
        LOGE(" %d", array[i + 1]);
    }
}



#endif //TESTJNI_PRIORITYQUEUE_HPP
