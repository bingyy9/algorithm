#ifndef ARRAY_LIST_H2
#define ARRAY_LIST_H2

template <class E>
class ArrayList{

private:
    E* array;  //数组指针
    int len = 0;
    int index = 0;

public:
    ArrayList();
    ArrayList(int len);
    void add(E e);
    E remove(int index);
    E get(int index);
    ~ArrayList();
};
#endif