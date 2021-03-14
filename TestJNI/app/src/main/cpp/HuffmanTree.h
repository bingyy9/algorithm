//
// Created by bingy on 2021/3/2.
//

#ifndef TESTJNI_HUFFMANTREE_H
#define TESTJNI_HUFFMANTREE_H

#include <string>
using namespace std;
typedef struct _hNode{
    char symbol;
    char *code;
    struct _hNode *next;
    struct _hNode* left;
    struct _hNode* right;
}hNode;

typedef struct _hTree{
    hNode* root;
}hTree;

typedef struct _hTable{
    hNode* first;
    hNode* last;
}hTable;

class HuffmanTree{
public:
    hTree* createHuffmanTree(char* str);
    hTable* createHuffmanTable(hTree* tree);
    void encode(hTable* table, char* str, std::string *encode_str);
    void decode(hTree* tree, char* str);

private:
    void traverseTree(hNode *pNode, hTable **pTable, int k, char code[256]);
};

#endif //TESTJNI_HUFFMANTREE_H
