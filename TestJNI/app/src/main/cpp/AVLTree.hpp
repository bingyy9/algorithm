//
// Created by bingy on 2021/2/26.
//

#ifndef TESTJNI_AVLTREE_HPP
#define TESTJNI_AVLTREE_HPP

template <class K, class V>
struct AVLTree_Node{
    K key;
    V data;
    AVLTree_Node<K, V>* left;
    AVLTree_Node<K, V>* right;
    int height;

    AVLTree_Node(K key, V value): height(1){  //默认创建的第一个节点高度就是1
        this->key = key;
        this->data = value;
        this->left = NULL;
        this->right = NULL;
    }

    AVLTree_Node(AVLTree_Node<K, V>* node) : height(node->height){
        this->left = node->left;
        this->right = node->right;
        this->key = node->key;
        this->data = node->data;
    }
};

template <class K, class V>
class AVLTree{
private:
    int count;
public:
    AVLTree_Node<K, V> *root;
public:
    AVLTree(){
        count = 0;
        root = NULL;
    }

    ~AVLTree(){

    }

    int get_height(AVLTree_Node<K, V>* node){
        return node? node->height : 0;
    }

    AVLTree_Node<K, V>* L_R_Rotation(AVLTree_Node<K, V>* node){
        LOGE("L_R_Rotation for node: %d", node->data);
        node->left = L_Rotation(node->left);
        return R_Rotation(node);
    }

    AVLTree_Node<K, V>* R_L_Rotation(AVLTree_Node<K, V>* node){
        LOGE("R_L_Rotation for node: %d", node->data);
        node->right = R_Rotation(node->right);
        return L_Rotation(node);
    }

    AVLTree_Node<K, V>* R_Rotation(AVLTree_Node<K, V>* node){
        LOGE("R_Rotation for node: %d", node->data);
        AVLTree_Node<K, V>* left = node->left;
        AVLTree_Node<K, V>* right = left->right; //如果右结点不为空，需要让node->left = right
        left->right = node;
        node->left = right;

        node->height = max(get_height(node->left), get_height(node->right)) + 1;
        left->height = max(get_height(left->left), get_height(left->right)) + 1;
        return left;
    }

    AVLTree_Node<K, V>* L_Rotation(AVLTree_Node<K, V>* node){
        LOGE("L_Rotation for node: %d", node->data);
        AVLTree_Node<K, V>* right = node->right;
        AVLTree_Node<K, V>* left = right->left;
        right->left = node;
        node->right = left;

        node->height = max(get_height(node->left), get_height(node->right)) + 1;
        right->height = max(get_height(right->left), get_height(right->right)) + 1;
        return right;
    }

    //理解这里的递归为什么需要返回一个指针*****
    AVLTree_Node<K, V>* addNode_(AVLTree_Node<K, V>* node, K key, V value){
        if(node == NULL){
            count++;
            return new AVLTree_Node<K, V>(key, value);
        }

        if(key < node->key){
            node->left = addNode_(node->left, key, value);

            int left_depth = get_height(node->left);
            int right_depth = get_height(node->right);
            if(left_depth - right_depth == 2){
                //need adjust for AVL
                if(get_height(node->left->right) > get_height(node->left->left)){
                    //不能简简单单只做一次右旋，比如2，0，1的case，需要先左旋，在右旋
                    node = L_R_Rotation(node);
                } else {
                    node = R_Rotation(node);
                }
            }
        } else if(key > node->key){
            node->right = addNode_(node->right, key, value);
            int left_depth = get_height(node->left);
            int right_depth = get_height(node->right);
            if(right_depth - left_depth == 2){
                //need adjust for AVL
                if(get_height(node->right->left) > get_height(node->right->right)){
                    node = R_L_Rotation(node);
                } else {
                    node = L_Rotation(node);
                }
            }
        } else if(key == node->key){
            node->data = value;
        }
        node->height = max(get_height(node->right), get_height(node->left)) + 1;
        return node;
    }

    //递归
    void put2(K key, V value){
        root = addNode_(root, key, value);
    }

    void put(K key, V value){
        if(root == NULL){
            root = new AVLTree_Node<K, V>(key, value);
            count++;
            return;
        }

        AVLTree_Node<K, V> *node = root;
        while(node != NULL){
            if(key < node->key){
                if(node->left != NULL){
                    node = node->left;
                    continue;
                } else {
                    node->left = new AVLTree_Node<K, V>(key, value);
                    count++;
                    break;
                }
            } else if(key > node->key){
                if(node->right != NULL){
                    node = node->right;
                    continue;
                } else {
                    node->right = new AVLTree_Node<K, V>(key, value);
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

    AVLTree_Node<K, V> *removeNode(AVLTree_Node<K, V> *node, K key){
        if(node == NULL){
            return NULL;
        }
        if(key < node->key){
            node->left = removeNode(node->left, key);

            if(get_height(node->right) - get_height(node->left) == 2){
                if(get_height(node->right->left) > get_height(node->right->right)){
                    node = R_L_Rotation(node);
                } else {
                    node = L_Rotation(node);
                }
            }
        } else if(key > node->key){
            node->right = removeNode(node->right, key);

            int left_depth = get_height(node->left);
            int right_depth = get_height(node->right);
            if(left_depth - right_depth == 2){
                //need adjust for AVL
                if(get_height(node->left->right) > get_height(node->left->left)){
                    node = L_R_Rotation(node);
                } else {
                    node = R_Rotation(node);
                }
            }
        } else if(key == node->key){
            //remove
            count--;
            if(node->left == NULL && node->right == NULL){
                //待删除节点的子节点
                delete node;
                return NULL;
            } else if(node->left != NULL && node->right == NULL){
                //待删除节点只有左结点
                AVLTree_Node<K, V> * tmp = node->left; //保存在释放
                delete node;
                return tmp;
            } else if(node->right != NULL && node->left == NULL){
                //待删除节点只有右结点
                AVLTree_Node<K, V> * tmp = node->right; //保存在释放
                delete node;
                return tmp;
            } else if(node->left != NULL && node->right != NULL){
                //待删除节点有左右结点，先找到左子树中最大结点，断开此结点连接，把此结点取代带删除结点。
                if(get_height(node->left) > get_height(node->right)){
                    //左边找最大值移除
                    AVLTree_Node<K, V> *left_max_node = max_node(node->left);
                    AVLTree_Node<K, V> *successor = new AVLTree_Node<K, V>(left_max_node); //下面disconnect_max会释放left_max_node
                    successor->left = removeNode(node->left, left_max_node->key); //这样删除的同时也会更新高度 //disconnect_max(node->left);
                    count++;
                    successor->right = node->right;
                    delete node;

                    //删除需要更新子结点高度
                    successor->height = max(get_height(successor->left), get_height(successor->right)) + 1;
                    return successor;
                } else {
                    //右边找最小值移除
                    AVLTree_Node<K, V> *right_min_node = min_node(node->right);
                    AVLTree_Node<K, V> *successor = new AVLTree_Node<K, V>(right_min_node); //下面disconnect_max会释放left_max_node
                    successor->right = removeNode(node->right, right_min_node->key); //这样删除的同时也会更新高度 //disconnect_max(node->left);
                    count++;
                    successor->left = node->left;
                    delete node;

                    //删除需要更新子结点高度
                    successor->height = max(get_height(successor->left), get_height(successor->right)) + 1;
                    return successor;
                }
            }
        }

        node->height = max(get_height(node->left), get_height(node->right)) + 1;
        return node;
    }

    //查找当前树的最大值，不断的往右边找
    AVLTree_Node<K, V>* max_node(AVLTree_Node<K, V> *node){
        AVLTree_Node<K, V> *tmp = node;
        while(tmp->right != NULL){
            tmp = tmp->right;
        }
        return tmp;
    }

    AVLTree_Node<K, V>* min_node(AVLTree_Node<K, V> *node){
        AVLTree_Node<K, V> *tmp = node;
        while(tmp->left != NULL){
            tmp = tmp->left;
        }
        return tmp;
    }

    AVLTree_Node<K, V> * disconnect_max(AVLTree_Node<K, V> *node){
        if(node->right == NULL){
            AVLTree_Node<K, V> *left = node->left;
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

    bool contains_(AVLTree_Node<K, V>* node, K key){
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

    void in_order_traverse(AVLTree_Node<K, V>* node){
        if(node == NULL){
            return;
        }

        in_order_traverse(node->left);
        LOGE("%d", node->data);
        in_order_traverse(node->right);
    }

    void layer_traverse(){
        if(this->root == NULL){
            return;
        }

        std::queue<AVLTree_Node<K, V>*> nodes;
        nodes.push(this->root);
        while(!nodes.empty()) {
            AVLTree_Node<K, V> *node1 = nodes.front();
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
