//
// Created by bingy on 2021/3/2.
//

#include "HuffmanTree.h"
#include "malloc.h"
#include "pQueue.h"
#include <cstring>

#include <android/log.h>

#define TAG "JNI_TAG"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
using namespace std;

hTree* HuffmanTree::createHuffmanTree(char* str){
    int* probability = (int *)malloc(sizeof(int) * 256);
    for(int i=0; i<256; i++){
        probability[i] = 0;
    }

    for(int i=0; str[i] != '\0'; i++){
        probability[(unsigned char)str[i]]++;
    }

    pQueue* huffmanQueue;
    initPQueue(&huffmanQueue);

    for(int i=0; i<256; i++){
        if(probability[i] != 0){
            hNode* aux = (hNode*)malloc(sizeof(hNode));
            aux->left = NULL;
            aux->right = NULL;
            aux->symbol = (char)i;

            addPQueue(&huffmanQueue, aux, probability[i]);
        }
    }

    free(probability);

    while(huffmanQueue->size != 1){
        int priority = huffmanQueue->first->priority;
        priority += huffmanQueue->first->next->priority;

        hNode *left = pop(&huffmanQueue);
        hNode *right = pop(&huffmanQueue);

        hNode *new_node = (hNode*)malloc(sizeof(hNode));
        new_node->left = left;
        new_node->right = right;

        addPQueue(&huffmanQueue, new_node, priority);
    }

    hTree *tree = (hTree*)malloc(sizeof(hTree));
    tree->root = huffmanQueue->first->val;
    return tree;
}


hTable* HuffmanTree::createHuffmanTable(hTree* tree){
    hTable * table = (hTable*)malloc(sizeof(hTable));
    table->first = NULL;
    table->last = NULL; //这里为什么要有last指针？尾插

    char code[256];
    int k = 0;
    traverseTree(tree->root, &table, k, code);
    return table;
}
void HuffmanTree::encode(hTable* table, char* str, string *encode_str){
    hNode *traversal;
    for(int i=0; str[i] != '\0'; i++){
        traversal = table->first;
        while(traversal->symbol != str[i]){
            traversal = traversal->next;
        }
        LOGE("%c encode to %s", traversal->symbol, traversal->code);
        encode_str->append(traversal->code);
    }
}

void HuffmanTree::traverseTree(hNode *treeNode, hTable **table, int k, char *code) {
    if(treeNode->left == NULL && treeNode->right == NULL){
        code[k] = '\0';
        hNode *aux = (hNode*)malloc(sizeof(hNode));
        aux->code = (char*)malloc(sizeof(char) * (strlen(code) + 1));
        strcpy(aux->code, code);
        aux->symbol = treeNode->symbol;
        aux->next = NULL;
        if((*table)->first == NULL){
            (*table)->first = aux;
            (*table)->last = aux;
        } else {
            (*table)->last->next = aux;
            (*table)->last = aux;
        }
    }

    if(treeNode->left != NULL){
        code[k] = '0';
        traverseTree(treeNode->left, table, k+1, code);
    }

    if(treeNode->right != NULL){
        code[k] = '1';
        traverseTree(treeNode->right, table, k+1, code);
    }
}

