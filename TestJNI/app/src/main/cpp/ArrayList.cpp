#include "ArrayList.h"
#include <malloc.h>

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