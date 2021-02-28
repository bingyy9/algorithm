//
// Created by bingy on 2021/2/4.
//

#ifndef TESTJNI_SORTUTILS_H
#define TESTJNI_SORTUTILS_H

class SortUtils{
public:
    static void bubbleSort(int* inputArr, int len);
    static void optimize_bubbleSort(int* arr, int len);
    static void selectSort(int* inputArr, int len);
    static void insert_sort(int* arr, int len);
    static void insert_sort_optimize(int* arr, int len);
    static void hill_sort(int* arr, int len);
    static void quick_sort(int* arr, int len);
    static void merge_sort(int* arr, int len);
    static void heap_sort(int* arr, int len);
    static int *create_random_array(int len, int low, int high);
    static void sort_array(char* sortName, void(*sort)(int*, int), int *arr, int len);
    static int* copy_rand_array(int* arr, int len);
    static void print_array(int* arr, int len);


private:
};

#endif //TESTJNI_SORTUTILS_H
