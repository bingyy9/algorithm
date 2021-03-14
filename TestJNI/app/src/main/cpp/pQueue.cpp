//
// Created by bingy on 2021/3/2.
//

#include "pQueue.h"
#include "malloc.h"

#include <android/log.h>
#define TAG "JNI_TAG"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
using namespace std;

void initPQueue(pQueue **queue){
    (*queue) = (pQueue *)malloc(sizeof(pQueue));
    (*queue)->first = NULL;
    (*queue)->size = 0;
}

void addPQueue(pQueue **queue, TYPE val, unsigned int priority){
    if((*queue)->size == MAX_SZ){
        LOGE("queue is full!");
        return;
    }

    pQueueNode *aux = (pQueueNode *)malloc(sizeof(pQueueNode));
    aux->priority = priority;
    aux->val = val;

    if((*queue)->size == 0 || (*queue)->first == NULL){
        aux->next = NULL;
        (*queue)->first = aux;
        (*queue)->size++;
        return;
    } else {
        if(priority <= (*queue)->first->priority){
            aux->next = (*queue)->first;
            (*queue)->first = aux;
            (*queue)->size++;
            return;
        } else {
            pQueueNode *node = (*queue)->first;
            while(node->next != NULL){
                if(priority <= node->next->priority){
                    aux->next = node->next;
                    node->next = aux;
                    (*queue)->size++;
                    return;
                }
                node = node->next;
            }

            if(node->next == NULL){
                aux->next = node->next;
                node->next = aux;
                (*queue)->size++;
                return;
            }
        }
    }
}

TYPE pop(pQueue **queue){
    TYPE value = NULL;
    if((*queue)->size > 0){
        value = (*queue)->first->val;
        (*queue)->first = (*queue)->first->next;
        (*queue)->size--;
    } else {
        LOGE("queue is empty");
    }
    return value;
}