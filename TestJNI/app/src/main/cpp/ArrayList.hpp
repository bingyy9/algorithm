#ifndef ARRAY_LIST_H2
#define ARRAY_LIST_H2

//声明
template <class E>
class ArrayList{

private:
    E* array;  //数组指针
    int len = 0;  //实际长度
    int index = 0;  //当前可存储元素index

public:
    ArrayList();
    ArrayList(int len);
    void add(E e);
    E remove(int index);
    E get(int index);
    int size();
    ~ArrayList();

private:
    void ensureCapacityInternal(int capacity);
    void grow(int capacity);
};
#endif

#include <malloc.h>
//实现
template <class E>
ArrayList<E>::ArrayList(){

};

template <class E>
ArrayList<E>::ArrayList(int len){
    if(len <= 0){
        return;
    }
    this->len = len;
    this->array = (E*)malloc(sizeof(E)*len);
};

template <class E>
ArrayList<E>::~ArrayList(){
    if(this->array){
        free(this->array);
        this->array = NULL;
    }

    this->len = 0;
    this->index = 0;
}

template <class E>
void ArrayList<E>::ensureCapacityInternal(int capacity){
    if(this->array == NULL){
        capacity = 10;
    }

    if(capacity - len > 0){
        grow(capacity);
    }
};

template <class E>
void ArrayList<E>::grow(int capacity){
    int new_len = len + (len >> 1);
    if(capacity - new_len > 0){
        new_len = capacity;
    }

    //create new array
    E* new_arr = (E*)malloc(sizeof(E) * new_len);
    if(this->array){
        //copy to new array
        memcpy(new_arr, this->array, sizeof(E)*index);
        free(this->array);
        this->array = NULL;
    }

    this->array = new_arr;
    this->len = new_len;
}

template <class E>
void ArrayList<E>::add(E e){
    if(e == NULL){
        return;
    }
    ensureCapacityInternal(index + 1);
    this->array[index++] = e;
}

template <class E>
int ArrayList<E>::size(){
    return this->index;
}

template <class E>
E ArrayList<E>::get(int index){
    return this->array[index];
}