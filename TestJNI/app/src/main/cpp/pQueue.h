//
// Created by bingy on 2021/3/2.
//

#ifndef TESTJNI_PQUEUE_H
#define TESTJNI_PQUEUE_H

#include "HuffmanTree.h"

#define TYPE hNode*
#define MAX_SZ 256
typedef struct _pQueueNode{
    TYPE val;
    unsigned int priority;
    struct _pQueueNode* next;
}pQueueNode;

typedef struct _pQueue{
    unsigned int size;
    pQueueNode *first;
}pQueue;

void initPQueue(pQueue **queue);
void addPQueue(pQueue **queue, TYPE val, unsigned int priority);
TYPE pop(pQueue **queue);


#endif //TESTJNI_PQUEUE_H
