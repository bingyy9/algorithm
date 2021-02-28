//
// Created by bingy on 2021/2/20.
//

#ifndef TESTJNI_LINKSTACK_HPP
#define TESTJNI_LINKSTACK_HPP

template <class E>
struct Node{
    E value;
    Node* pre;
    Node* next;

    Node(E v, Node* p, Node* n){
        value = v;
        pre = p;
        next = n;
    }
};

template <class E>
class LinkStack{
private:
    Node<E>* head = NULL;
    Node<E>* last = NULL;
    int len = 0;
public:
public:
    LinkStack();
    ~LinkStack();
    bool isEmpty();
    E pop();
    void push(E e);
    E pick(); //获取但不弹出
};

template <class E>
LinkStack<E>::LinkStack(){
}

template <class E>
LinkStack<E>::~LinkStack(){
    if(head == NULL){
        return;
    }

    Node<E> *node = head;
    Node<E> *next;
    while(node != NULL){
        next = node->next;
        delete(node);
        node = next;
    }
    head = NULL;
    last = NULL;
}

template <class E>
bool LinkStack<E>::isEmpty(){
    return len == 0;
}

//template <class E>
//E LinkStack<E>::pop(){
//    assert(len > 0);
//    return last;
//}
//
//template <class E>
//E LinkStack<E>::pick(){
//    return array[top];
//}

template <class E>
void LinkStack<E>::push(E e){
    Node<E> node = new Node<E>(e, NULL, NULL);
    if(head){
        this->last->next = node;
        node.pre = this->last;
        this->last = node;
    } else {
        head->next = node;
        node.pre = head;
        last = node;
    }
    len++;
}

#endif //TESTJNI_LINKSTACK_HPP
