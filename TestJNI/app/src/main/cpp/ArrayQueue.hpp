//
// Created by bingy on 2021/2/21.
//

#ifndef TESTJNI_ARRAYQUEUE_HPP
#define TESTJNI_ARRAYQUEUE_HPP

#include <malloc.h>

template <class E>
class ArrayQueue{
private:
    E* array = NULL;
    int head = 0;
    int tail = 0;
    //2的幂次，方便位运算
    int size = 0;

public:
    ArrayQueue();
    ArrayQueue(int size);
    ~ArrayQueue();
    bool isEmpty();
    //pop from head
    E pop();
    //push to queue tail
    void push(E e);

private:
    void grow();
    void copyElement(E* src, int src_pos, E* dest, int des_pos, int size);
};

template <class E>
ArrayQueue<E>::ArrayQueue() : ArrayQueue(4){

}

template <class E>
ArrayQueue<E>::ArrayQueue(int size){
    //size是可以乱传的，如何保证是2的幂次？  3->4  6->8  17->32
    int init_size = size;
    if(init_size >= 4){
        init_size |= init_size >> 1;
        init_size |= init_size >> 2;
        init_size |= init_size >> 4;
        init_size |= init_size >> 8;
        init_size |= init_size >> 16;
        init_size += 1;
    }
    this->size = init_size;
    array = (E*)malloc(this->size*sizeof(E));
}

template <class E>
ArrayQueue<E>::~ArrayQueue(){
    if(array){
        free (array);
        array = NULL;
    }

    head = NULL;
    tail = NULL;
    size = 0;
}

template <class E>
void ArrayQueue<E>::push(E e){
    head = (head - 1) & (size - 1);
    array[head] = e;
    if(tail == head){
        //双端队列，可在头部增加删除，也可在尾部增加删除
        grow();
    }
}

template <class E>
E ArrayQueue<E>::pop(){
    tail = (tail - 1) & (size - 1);
    return array[tail];
}

template <class E>
bool ArrayQueue<E>::isEmpty(){
    return tail == head;
}

template<class E>
void ArrayQueue<E>::grow() {
    int new_size = size << 1;
    E* new_array = (E*) malloc(sizeof(E) * new_size);
    //对数据进行copy，将tail后面的copy到前面。将tail前面的的拷贝到后面
    int r = size - tail;
    copyElement(array, tail, new_array, 0, r);
    copyElement(array, 0, new_array, r, tail);
    free(array);

    head = 0;
    tail = new_size;
    array = new_array;
}


template<class E>
void ArrayQueue<E>::copyElement(E* src, int src_pos, E* dest, int des_pos, int size) {
    for(int i = 0; i<size; i++){
        dest[des_pos + i] = src[src_pos + i];
    }
}


#endif //TESTJNI_ARRAYQUEUE_HPP
