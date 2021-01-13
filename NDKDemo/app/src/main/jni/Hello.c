#include <studio.h>
#include <stdlib.h>
#include <jni.h>

jstring Java_com_example_ndkdemo_JNI_sayHello(JNIEnv* env, jobject jobj){
    char* text = "I am from C";
    return (*env)-> NewStringUTF(e  nv, text);
}

