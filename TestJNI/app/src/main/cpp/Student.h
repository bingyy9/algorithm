//
// Created by bingy on 2021/1/28.
//

class Student {
private:
    char *name;
    int age;
    static int tag;
    //Teacher is Student的友元类，Teacher里面就可以通过Student访问私有属性
    friend class Teacher;

public:
    Student();
    Student(int age);
    Student(char * name);
    Student(char * name, int age);
    ~Student();
    Student(const Student& stu);
    void setAge(int age);
    void setName(char* name);
    int getAge();
    char* getName() const;
    static void changeStudentTag(int tag1);
    friend void changeStudentAge(Student *stu, int age);
};