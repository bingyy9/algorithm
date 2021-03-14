//
// Created by bingy on 2021/2/28.
//

#ifndef TESTJNI_MAP2_HPP
#define TESTJNI_MAP2_HPP

typedef bool rb_color;
#define red true
#define black false

template <class K, class V>
class map2{
private:
    struct TreeNode_;
    int count;
    TreeNode_* root;
public:
    map2(): root(NULL){
        count = 0;
    }

    struct iterator;

    iterator insert(K key, V value){
        if(root == NULL){
            root = new TreeNode_(NULL, NULL, NULL, key, value, black);
            count = 1;
            return iterator(root);
        }

        //最好我们插入红色结点，它不会违反性质5，但是有可能违反性质4
        TreeNode_* node = root;
        TreeNode_* parent = NULL;
        while(node){
            parent = node;
            if(key < node->key){
                node = node->left;
            } else if(key > node->key){
                node = node->right;
            } else if(key == node->key){
                node->value = value;
                return iterator(node);
            }
        }

        count++;
        TreeNode_ *new_node = new TreeNode_(NULL, NULL, parent, key, value, red);
        if(key > parent->key){
            parent->right = new_node;
        } else {
            parent->left = new_node;
        }

        solveDoubleRed(new_node);

        return iterator(new_node);

    }

    TreeNode_* parent(TreeNode_* node){
        return node? node->parent : NULL;
    }

    TreeNode_* brother(TreeNode_* node){
//        return left(parent(node)) == node? right(parent(node)) : left(parent(node));
        TreeNode_* parent = this->parent(node);
        if(parent && parent->left == node){
            //当前是左儿子，兄弟就是右儿子
            return parent->right;
        } else if(parent && parent->right == node){
            return parent->left;
        } else {
            return NULL;
        }
    }

    rb_color getColor(TreeNode_ *node){
        return node? node->color: black;
    }

    void setColor(TreeNode_ *node, rb_color color){
        if(node){
            node->color = color;
        }
    }

    TreeNode_ *left(TreeNode_ *node){
        return node? node->left : NULL;
    }

    TreeNode_ *right(TreeNode_ *node){
        return node? node->right : NULL;
    }

    TreeNode_* L_Rotation(TreeNode_ *node){
        LOGE("node %d: L_Rotation", node->value);
        //左旋，多了一个parent结点
        TreeNode_* right = node->right;
        node->right = right->left;
        right->left = node;

        //调整父亲的儿子指向
        if(node->parent == NULL){
            root = right;
        } else if(node->parent->left == node){
            node->parent->left = right;
        } else {
            node->parent->right = right;
        }

        //调整各个节点的父亲
        right->parent = node->parent;
        node->parent = right;
        if(node->right){
            node->right->parent = node;
        }
        return right;
    }

    TreeNode_* R_Rotation(TreeNode_ *node){
        LOGE("node %d: R_Rotation", node->value);
        TreeNode_* left = node->left;
        node->left = left->right; //如果右结点不为空，需要让node->left = right
        left->right = node;

        if(node->parent == NULL){
            root = left;
        } else if(node->parent->left == node){
            node->parent->left = left;
        } else {
            node->parent->right = left;
        }

        left->parent = node->parent;
        node->parent = left;
        if(node->left){
            node->left->parent = node;
        }
        return left;
    }

    void solveDoubleRed(TreeNode_ *node){
        //双红的解决有三种：
        //1. 父亲结点颜色是黑色的，不需要解决。
        // 父结点颜色是红色，需要调整
        //1）叔叔结点是红色的，把叔叔结点变黑，把父亲结点变黑，然后指针回溯到爷爷，爷爷染红
        //2）如果叔叔是黑色的，把父亲染黑，把爷爷染红，右旋爷爷。 （空节点也是黑色的）
        //•	如果当前结点是父亲的左儿子，父亲左旋就可以了
        //•	当前结点是父亲的右儿子，需要先把父亲左旋，在把爷爷右旋
        while(node->parent && node->parent->color == red){
            if(getColor(brother(parent(node))) == red){
                setColor(brother(parent(node)), black);
                setColor(parent(node), black);
                setColor(parent(parent(node)), black);
                node = parent(parent(node)); //指针回溯至爷爷
            } else if(getColor(brother(parent(node))) == black){
                if(parent(parent(node))->left == parent(node)){
                    if(right(parent(node)) == node){
//                        node = parent(node);
                        L_Rotation(parent(node));
                    }

                    setColor(parent(node), black);
                    setColor(parent(parent(node)), red);

                    R_Rotation(parent(parent(node)));
                } else if(parent(parent(node))->right = parent(node)){
                    if(left(parent(node)) == node){
//                        node = parent(node);
                        R_Rotation(parent(node));
                    }
                    setColor(parent(node), black);
                    setColor(parent(parent(node)), red);
                    L_Rotation(parent(parent(node)));
                }

            }
        }

        root->color = black;
    }

    TreeNode_* findTree(K key){
        if(root == NULL){
            return NULL;
        }

        TreeNode_* node = root;
        while(node != NULL){
            if(key < node->key){
                node = node->left;
            } else if(key > node->key){
                node = node->right;
            } else if(key == node->key){
                return node;
            }
        }

        return NULL;
    }

    void solveLostBlack(TreeNode_* node){
        //问题是失去了一个黑色结点，需要想办法去兄弟树上拿一个红色结点染黑
        //1. 如果兄弟结点是红色的，把兄弟结点染黑，父节点染红，左、右旋父节点
        //2. 如果兄弟结点是黑色的，并且两个侄子结点都是黑色的，将两个侄子结点染红，指针回溯至父节点
        //3. 如果兄弟结点是黑色的并且进侄子是红色的，将进侄子染黑，兄弟染红，左右旋兄弟结点，进入下面情况4
        //4. 如果兄弟结点是黑色的并且远侄子是红色的，将兄弟结点染成父节点的颜色，父节点染黑，远侄子染黑，左右旋父节点
//        while(node != root && node->color == black){
//            if(left(parent(node)) == node){
//                TreeNode_*
//            } else {
//
//            }
//        }

        //当遇到一个红色结点，把红色结点染黑即可
        node->color = black;
    }

    bool remove(K key){
        TreeNode_* current = findTree(key);
        if(current == NULL){
            return false;
        }

        if(current->left != NULL && current->right != NULL){
            TreeNode_ *successor = current->successor();
            current->key = successor->key;
            current->value = successor->value;
            current = successor;
        }

        if(current->left != NULL){
            if(current->parent->left == current){
                current->parent->left = current->left;
            } else {
                current->parent->right = current->left;
            }

            current->left->parent = current->parent;
            if(current->color == black){
                solveLostBlack(current->left);
            }

            delete(current);
        } else if(current->right != NULL){
            if(current->parent->left == current){
                current->parent->left = current->right;
            } else {
                current->parent->right = current->right;
            }

            current->right->parent = current->parent;
            if(current->color == black){
                solveLostBlack(current->right);
            }

            delete(current);
        } else {
            if(current->color == black){
                solveLostBlack(current);
            }

            if(current->parent){
                if(current->parent->left == current) {
                    current->parent->left = NULL;
                } else {
                    current->parent->right = NULL;
                }
            }
            delete(current);
        }

        count--;
        return true;
    }

    int size(){
        return count;
    }

    bool is_empty(){
        return count == 0;
    }

    void layer_traverse(){
        if(root == NULL){
            return;
        }

        std::queue<TreeNode_*> q;
        q.push(root);
        while(!q.empty()){
            TreeNode_* node = q.front();
            q.pop();
            if(node->color == red){
                LOGE("node: %d, red ", node->value);
            } else {
                LOGE("node: %d, black", node->value);
            }

            if(node->left){
                q.push(node->left);
            }

            if(node->right){
                q.push(node->right);
            }
        }
    }

};

template <class K, class V>
struct map2<K, V>::TreeNode_{
public:
    TreeNode_* left;
    TreeNode_* right;
    TreeNode_* parent;
    K key;
    V value;
    rb_color color;

public:
    TreeNode_(TreeNode_* left, TreeNode_* right, TreeNode_* parent, K key, V value, rb_color color)
        : left(left), right(right), parent(parent), key(key), value(value), color(color){}

    //找前驱：找左子树的最大值
    TreeNode_ *precursor(){
        if(left){
            TreeNode_* node = left;
            while(node->right){
                node = node->right;
            }
            return node;
        } else {
            TreeNode_* preccursor = this;
            while(preccursor->parent && preccursor->parent->left == preccursor){
                preccursor = preccursor->parent;
            }
            return preccursor->left;
        }
    }

    //找后继：找右子树最小值.  如果没有右子树，就不断的往上找
    TreeNode_* successor(){
        if(right){
            TreeNode_* node = right;
            while(node->left){
                node = node->left;
            }
            return node;
        } else {
            TreeNode_* successor = this;
            while(successor->parent && successor->parent->right == successor){
                successor = successor->parent;
            }
            return successor->parent;
        }
    }
};

template <class K, class V>
struct map2<K,V>::iterator{
private:
    TreeNode_* node;
public:
    iterator(TreeNode_* node){

    }

    iterator& operator--(){ //找前驱
        node = node->precursor();
        return *this;
    }

    iterator& operator++(){ //找后继
        node = node->successor();
        return *this;
    }

    V operator*(){
        return node->value;
    }
};

#endif //TESTJNI_MAP2_HPP
