//
// Created by bingy on 2021/1/28.
//
#ifndef STUDENT_H
#define STUDENT_H

#include <android/log.h>
#include "Student.h"
#define TAG "JNI_TAG"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

Student::Student(){
    age = 10;
    name = "123";
}

//Student::Student(char* name) : age(0){  //一个参数的构造函数
//    this->name = name;
////    this->age = 0;
//}

Student::Student(char* name) : Student(name, 10){  //构造函数的相互调用
    //先会调用2个参数的构造函数，才会执行当前构造函数。
}

Student::Student(int age){
    this->age = age;
};

Student::Student(char* name, int age){
    this->name = name;
    this->age = age;
};

Student::~Student(){
}

//拷贝构造函数，对象会有一个默认的拷贝构造函数，用于对象间的赋值
Student::Student(const Student& stu){
    LOGE("copy construct");
    this->age = stu.age;
    this->name = stu.name;
}

void Student::setAge(int age){
    this->age = age;
}

void Student::setName(char* name){
    this->name = name;
}

int Student::getAge(){
    return age;
}

char* Student::getName() const{
    return name;
}

//static field
int Student::tag = 1;
//static method
void Student::changeStudentTag(int tag1){
    tag = tag1;
}

//friend method don't need to append Student::
void changeStudentAge(Student *stu, int age){
    stu->age = age;
}

#endif