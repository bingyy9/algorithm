//
// Created by bingy on 2021/2/3.
//

#ifndef TESTJNI_ARRAY_LIST_H2
#define TESTJNI_ARRAY_LIST_H2

#include <assert.h>

template <class E>
struct Nod{
    E value;
    Nod* next;
    Nod* pre;
public:
    Nod(E value, Nod* pre, Nod* next){
        this->value = value;
        this->next = next;
        this->pre = pre;
    }
};

template <class E>
class LinkedList{
private:
    Nod<E>* head;
    Nod<E>* last;
    int len;

public:
    LinkedList();
    void push(E e);
    Nod<E>* node(int i);
    E get(int i);
    int size();
    void insert(int index, E e);
    E remove(int index);
    ~LinkedList();

private:
    void linkLast(E e);
};

#endif //TESTJNI_ARRAY_LIST_H2

template <class E>
void LinkedList<E>::push(E e){
    Nod<E> *node = new Nod<E>(e, NULL, NULL);
    if(head){//not NULL
        last->next = node;
        node->pre = last;
        last = node;
    } else {
        head = node;
        last = node;
    }
    len++;
}

template <class E>
void LinkedList<E>::linkLast(E e){

}

template <class E>
Nod<E>* LinkedList<E>::node(int index){
    if(index < len >> 1){
        Nod<E>* h = head; //不能改变头结点
        for(int i=0; i < index; i++){
            h = h->next;
        }
        return h;
    } else {
        Nod<E>* h = last; //不能改变头结点
        for(int i=(len - 1); i > index; i--){
            h = h->pre;
        }
        return h;
    }
}

template <class E>
int LinkedList<E>::size(){
    return len;
}

template <class E>
E LinkedList<E>::get(int index){
    assert(index >=0 && index <= len);
    Nod<E>* node = this->node(index);
    assert(node);
    if(node){
        return node->value;
    }
    return NULL;
}

template <class E>
void LinkedList<E>::insert(int index, E e){
    assert(index >=0 && index <= len);
    Nod<E>* new_node = new Nod<E>(e, NULL, NULL);
    if(index == 0){
        new_node->next = head;
        head->pre = new_node;
        head = new_node;
    } else {
        Nod<E>* prev_node = this->node(index - 1);
        if(prev_node->next){
            prev_node->next->pre = new_node;
        } else {

        }

        new_node->next = prev_node->next;
        new_node->pre = prev_node;
        prev_node->next = new_node;
    }
    len++;
}

template <class E>
E LinkedList<E>::remove(int index) {
    assert(index >= 0 && index < len);
    E value;
    Nod<E> *pending_remove_node = NULL;
    if (index == 0) {
        pending_remove_node = head;
        head = head->next;
        head->pre = NULL;
    } else {
        Nod<E> *prev_node = this->node(index - 1);
        pending_remove_node = prev_node->next;

        prev_node->next = pending_remove_node->next;
        if (pending_remove_node->next) {
            pending_remove_node->next->pre = prev_node;
        } else {
            //pending remove last node, need to modify last node as well
            last = prev_node;
        }
    }
    value = pending_remove_node->value;
    len--;
    delete(pending_remove_node);
    pending_remove_node = NULL;
    return value;
}


template <class E>
LinkedList<E>::LinkedList(){
    head = NULL; //必须初始化，否则不是NULL
    last = NULL;
    len = 0; //必须初始化，否则不是0
}

template <class E>
LinkedList<E>::~LinkedList(){
    if(head == NULL){
        return;
    }

    Nod<E> *node = head;
    Nod<E> *next;
    while(node != NULL){
        next = node->next;
        delete(node);
        node = next;
    }
    head = NULL;
    last = NULL;
}