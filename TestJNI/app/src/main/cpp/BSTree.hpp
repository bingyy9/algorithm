//
// Created by bingy on 2021/2/26.
//

#ifndef TESTJNI_BSTREE_HPP
#define TESTJNI_BSTREE_HPP

template <class K, class V>
struct Tree_Node{
    K key;
    V data;
    Tree_Node<K, V>* left;
    Tree_Node<K, V>* right;
    int height;

    Tree_Node(K key, V value): height(0){
        this->key = key;
        this->data = value;
        this->left = NULL;
        this->right = NULL;
    }

    Tree_Node(Tree_Node<K, V>* node) : height(node->height){
        this->left = node->left;
        this->right = node->right;
        this->key = node->key;
        this->data = node->data;
    }
};

template <class K, class V>
class BSTree{
private:
    int count;
public:
    Tree_Node<K, V> *root;
public:
    BSTree(){
        count = 0;
        root = NULL;
    }

    ~BSTree(){

    }

    int get_height(Tree_Node<K, V>* node){
        return node? node->height : 0;
    }

    //理解这里的递归为什么需要返回一个指针*****
    Tree_Node<K, V>* addNode_(Tree_Node<K, V>* node, K key, V value){
        if(node == NULL){
            count++;
            return new Tree_Node<K, V>(key, value);
        }

        if(key < node->key){
            node->left = addNode_(node->left, key, value);
        } else if(key > node->key){
            node->right = addNode_(node->right, key, value);
        } else if(key == node->key){
            node->data = value;
        }
        return node;
    }

    //递归
    void put2(K key, V value){
        root = addNode_(root, key, value);
    }

    void put(K key, V value){
        if(root == NULL){
            root = new Tree_Node<K, V>(key, value);
            count++;
            return;
        }

        Tree_Node<K, V> *node = root;
        while(node != NULL){
            if(key < node->key){
                if(node->left != NULL){
                    node = node->left;
                    continue;
                } else {
                    node->left = new Tree_Node<K, V>(key, value);
                    count++;
                    break;
                }
            } else if(key > node->key){
                if(node->right != NULL){
                    node = node->right;
                    continue;
                } else {
                    node->right = new Tree_Node<K, V>(key, value);
                    count++;
                    break;
                }
            } else if(key == node->key){
                node->data = value;
                break;
            }
        }
    }

    V* get(K key){
        //找不到返回什么？
        // 1. 函数的返回值应该是V的指针，这样找不到才能返回为NULL。
        // 2. 还有一种方式是先判断包不包含
    }

    Tree_Node<K, V> *removeNode(Tree_Node<K, V> *node, K key){
        if(node == NULL){
            return NULL;
        }
        if(key < node->key){
            node->left = removeNode(node->left, key);
        } else if(key > node->key){
            node->right = removeNode(node->right, key);
        } else if(key == node->key){
            //remove
            count--;
            if(node->left == NULL && node->right == NULL){
                //待删除节点的子节点
                delete node;
                return NULL;
            } else if(node->left != NULL && node->right == NULL){
                //待删除节点只有左结点
                Tree_Node<K, V> * tmp = node->left; //保存在释放
                delete node;
                return tmp;
            } else if(node->right != NULL && node->left == NULL){
                //待删除节点只有右结点
                Tree_Node<K, V> * tmp = node->right; //保存在释放
                delete node;
                return tmp;
            } else if(node->left != NULL && node->right != NULL){
                //待删除节点有左右结点，先找到左子树中最大结点，断开此结点连接，把此结点取代带删除结点。
                //待删除节点有左右结点，先找到左子树中最大结点，断开此结点连接，把此结点取代带删除结点。
                Tree_Node<K, V> *left_max_node = max_node(node->left);
                Tree_Node<K, V> *successor = new Tree_Node<K, V>(left_max_node); //下面disconnect_max会释放left_max_node
                successor->left = disconnect_max(node->left);
                successor->right = node->right;
                delete node;
                return successor;
            }
        }
        return node;
    }

    //查找当前树的最大值，不断的往右边找
    Tree_Node<K, V>* max_node(Tree_Node<K, V> *node){
        Tree_Node<K, V> *tmp = node;
        while(tmp->right != NULL){
            tmp = tmp->right;
        }
        return tmp;
    }

    Tree_Node<K, V> * disconnect_max(Tree_Node<K, V> *node){
        if(node->right == NULL){
            Tree_Node<K, V> *left = node->left;
            delete node;
            return left;
        }
        node->right = disconnect_max(node->right);
        return node;
    }

    void remove(K key){
        root = removeNode(root, key);
    }

    int size(){
        return count;
    }

    bool contains_(Tree_Node<K, V>* node, K key){
        if(node == NULL){
            return false;
        }

        if(node->key == key){
            return true;
        } else if(key < node->key){
            return contains_(node->left, key);
        } else {
            return contains_(node->right, key);
        }
    }

    bool contains(K key){
        return contains_(root, key);
    }

    void in_order_traverse(Tree_Node<K, V>* node){
        if(node == NULL){
            return;
        }

        in_order_traverse(node->left);
        LOGE("%d", node->data);
        in_order_traverse(node->right);
    }

    void layer_traverse(Tree_Node<K, V>* node){
        if(node == NULL){
            return;
        }

        std::queue<Tree_Node<K, V>*> nodes;
        nodes.push(node);
        while(!nodes.empty()) {
            Tree_Node<K, V> *node1 = nodes.front();
            LOGE("%d", node1->data);
            nodes.pop();

            if (node1->left) {
                nodes.push(node1->left);
            }
            if(node1->right){
                nodes.push(node1->right);
            }
        }
    }


};

#endif //TESTJNI_BSTREE_HPP
