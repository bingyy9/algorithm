//
// Created by bingy on 2021/2/25.
//

#ifndef TESTJNI_TREENODE_HPP
#define TESTJNI_TREENODE_HPP

#define TAG "JNI_TAG"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
using namespace std;

#include <queue>
#include <cmath>
#include <string>

template <class T>
class TreeNode{
private:
    T data;
public:
    TreeNode* left;
    TreeNode* right;
    TreeNode(T data);
    ~TreeNode();

    static void pre_order_traverse(TreeNode *);
    static void mid_order_traverse(TreeNode *);
    static void post_order_traverse(TreeNode *);
    static void layer_traverse(TreeNode *);
    static int get_tree_depth(TreeNode *);
    static bool is_avl_tree(TreeNode<T>* node);
    static void serialize_tree(TreeNode *, string* str);
    static TreeNode* deserialize_tree(char** str);
};

template <class T>
TreeNode<T>::TreeNode(T data){
    this->data = data;
    left = NULL;
    right = NULL;
}

template <class T>
TreeNode<T>::~TreeNode(){

}

template <class T>
void TreeNode<T>::pre_order_traverse(TreeNode<T>* node){
    if(node == NULL){
        return;
    }

    LOGE("%c", node->data);
    pre_order_traverse(node->left);
    pre_order_traverse(node->right);
}

template <class T>
void TreeNode<T>::mid_order_traverse(TreeNode<T>* node){
    if(node == NULL){
        return;
    }

    mid_order_traverse(node->left);
    LOGE("%c", node->data);
    mid_order_traverse(node->right);
}

template <class T>
void TreeNode<T>::post_order_traverse(TreeNode<T>* node){
    if(node == NULL){
        return;
    }

    post_order_traverse(node->left);
    post_order_traverse(node->right);
    LOGE("%c", node->data);
}

template <class T>
void TreeNode<T>::layer_traverse(TreeNode<T>* node){
    if(node == NULL){
        return;
    }
    std::queue<TreeNode<T>*> node_queue;
    node_queue.push(node);

    while(!node_queue.empty()){
        TreeNode<T>* n = node_queue.front();
        node_queue.pop();
        LOGE("%c", n->data);
        if(n->left){
            node_queue.push(n->left);
        }

        if(n->right){
            node_queue.push(n->right);
        }
    }
}

template <class T>
int TreeNode<T>::get_tree_depth(TreeNode<T>* node){
    if(node == NULL){
        return 0;
    }

    int left_depth = get_tree_depth(node->left);
    int right_depth = get_tree_depth(node->right);
    return max(left_depth, right_depth) + 1;
//    return left_depth > right_depth ? left_depth + 1 : right_depth + 1;
}

template <class T>
bool TreeNode<T>::is_avl_tree(TreeNode<T>* node){
    //1. 空树是平衡二叉树
    //2. 左右子树高度差<=1
    //3. 左右两个子树都是平衡二叉树
    if(node == NULL){
        return true;  //空树也是平衡二叉树
    }

    int left_depth = get_tree_depth(node->left);
    int right_depth = get_tree_depth(node->right);
    return abs(left_depth - right_depth) <= 1 && is_avl_tree(node->left) && is_avl_tree(node->right);
}


template <class T>
void TreeNode<T>::serialize_tree(TreeNode<T>* node, string* str){
    if(node == NULL){
        str->append("#");
        return;
    }
    str->append(string(1, node->data));
//    str += node->data;
    serialize_tree(node->left, str);
    serialize_tree(node->right, str);
}

//"ABD##E##C#FG###"
template <class T>
TreeNode<T>* TreeNode<T>::deserialize_tree(char** str){
    if(**str == '#'){
        *str += 1;
        return NULL;
    }
    TreeNode<T> *node = new TreeNode<T>(**str);
    *str += 1;
    node->left = deserialize_tree(str);
    node->right = deserialize_tree(str);
    return node;
}

#endif //TESTJNI_TREENODE_HPP
