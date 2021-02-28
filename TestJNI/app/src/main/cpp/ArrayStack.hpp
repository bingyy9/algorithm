#ifndef ARRAY_STACK_H2
#define ARRAY_STACK_H2

#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>

template <class E>
class ArrayStack{

private:
    int top = -1;
    E* array = NULL;
    int size = 10;
public:
    ArrayStack();
    ~ArrayStack();
    bool isEmpty();
    E pop();
    void push(E e);
    E pick(); //获取但不弹出
private:
    void growArray();
};

#endif

template <class E>
ArrayStack<E>::ArrayStack(){
    array = (E*)malloc(size*sizeof(E));
}

template <class E>
ArrayStack<E>::~ArrayStack(){
//    delete [] array;
    free(array);
}

template <class E>
bool ArrayStack<E>::isEmpty(){
    return top == -1;
}

template <class E>
E ArrayStack<E>::pop(){
    assert(top >= 0);
    return array[top--];
}

template <class E>
E ArrayStack<E>::pick(){
    return array[top];
}

template <class E>
void ArrayStack<E>::push(E e){
    //ensure capacity
    if(top + 1 == size){
        //enlarge
        growArray();
    }

    array[++top] = e;
}

template<class E>
void ArrayStack<E>::growArray() {
    int newsize = size + size >> 1;
    array = (E*)realloc(array, newsize*sizeof(E));
    size = newsize;
}






