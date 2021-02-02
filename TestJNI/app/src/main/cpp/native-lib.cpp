#include <jni.h>
#include <string>
#include <android/log.h>
#include "md5.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include "Student.h"
#include <stdarg.h>
#include "ArrayList.hpp"
#include <algorithm>
#include <cctype>
#include <vector>
#include <stack>
#include <queue>
#include <list>
#include <set>
#include <map>
#include <functional>

using namespace std;

#define TAG "JNI_TAG"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

using namespace std;

//extern "C" JNIEXPORT jstring JNICALL
//Java_com_example_testjni_MainActivity_stringFromJNI(
//        JNIEnv* env,
//        jobject /* this */) {
//    std::string hello = "Hello from C++";
//    return env->NewStringUTF(hello.c_str());
//}

//extern "C"{
//JNIEXPORT jstring JNICALL
//Java_com_example_testjni_SignatureUtils_signatureParams(JNIEnv *env, jclass clazz, jstring params_);
//}

static char* EXTRA_SIGNATURE = "DARREN";
static int is_verify = 0;
static char* PACKAGE_NAME = "com.example.testjni";
static char* APP_SIGNATURE = "308201dd30820146020101300d06092a864886f70d010105050030373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b3009060355040613025553301e170d3137313132343037353930335a170d3437313131373037353930335a30373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b300906035504061302555330819f300d06092a864886f70d010101050003818d00308189028181009804ab1cf9890b67deb141c9210a0ea2c36a646b848a51d3ae7e5659bf5101792a2359086ee1624923206a680cec8af6cabfd902d858d5cf6a28a344adb2322010baad10c476b105c9ab1998db4270873666237b92a3a414519adb132eec9851e322328ace0b401873644b390124bd41fddc28ea7af13ef5d8c27f5f286696530203010001300d06092a864886f70d01010505000381810088aed25c314caf1e3cf0407bfd4991dafc75dcc19b42cd352001d38e7bc68c6718728b30c5914a9962c171e532650da39bde593ebc862691bd5d98e1bd510ea35f89d523b367a4b9999d5ee1664033f86b17cdf127217cf8f80b7bd3d5f4c505c069270ab081d823541b639bfbba140bb42d14ca847791f36cc2c177a6a7f356";

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_testjni_SignatureUtils_signatureParams(JNIEnv *env, jclass clazz, jstring params_) {
    // TODO: implement signatureParams()
    if(is_verify == 0){
        return env->NewStringUTF("error");
    }

    const char *params = env->GetStringUTFChars(params_, 0);

    string signature_str(params);
    signature_str.insert(0, EXTRA_SIGNATURE);
    signature_str = signature_str.substr(0, signature_str.length() - 2);

    MD5_CTX *ctx = new MD5_CTX ();
    MD5Init(ctx);
    MD5Update(ctx, (unsigned char *) signature_str.c_str(), signature_str.length());
    unsigned char digest[16];
    MD5Final(ctx, digest);

    char md5_str[34] = {0};
//    printf("before encrypt: ", digest);
    for(int i = 0; i < 16; i++){
        //不足的情况下补0   f=0f, ab=ab
        sprintf(md5_str, "%s%02x", md5_str, digest[i]);
    }

    //释放资源
    env->ReleaseStringUTFChars(params_, params);
    return env->NewStringUTF(md5_str);
}


/*
 * PackageInfo packageInfo = null;
        try {
            packageInfo = this.getPackageManager().getPackageInfo(this.getPackageName(), PackageManager.GET_SIGNATURES);
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        Signature[] signatures = packageInfo.signatures;
        Log.e(TAG, "signature: " + signatures[0].toCharsString());

*/
extern "C" JNIEXPORT void JNICALL
Java_com_example_testjni_SignatureUtils_signatureVerify(JNIEnv *env, jclass clazz, jobject context) {
    //1.获取包名, 比对包名
    jclass  j_clz = env->GetObjectClass(context);
    jmethodID  j_mid = env->GetMethodID(j_clz, "getPackageName", "()Ljava/lang/String;");
    jstring j_package_name = (jstring)env->CallObjectMethod(context, j_mid);

    const char * c_package_name = env->GetStringUTFChars(j_package_name, NULL);
    if(strcmp(c_package_name, PACKAGE_NAME) != 0){
        return;
    }

    __android_log_print(ANDROID_LOG_ERROR, "JNI_TAG", "package name verified. %s", c_package_name);
    //3. 获取签名，比对签名是否一样
    //3.1 获取PackageManager
    j_mid = env->GetMethodID(j_clz, "getPackageManager", "()Landroid/content/pm/PackageManager;");
    jobject package_manager = env->CallObjectMethod(context, j_mid);

    //3.2 获取PackageInfo
    jclass j_package_manager_clz = env->GetObjectClass(package_manager);
    j_mid = env->GetMethodID(j_package_manager_clz, "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jobject package_info = env->CallObjectMethod(package_manager, j_mid, j_package_name, 0x00000040);

    //3.3 获取signatures数组
    j_clz = env->GetObjectClass(package_info);
    jfieldID j_fid = env->GetFieldID(j_clz, "signatures", "[Landroid/content/pm/Signature;");
    jobjectArray signatures = (jobjectArray)env->GetObjectField(package_info, j_fid);

    //3.4 获取signatures[0]
    jobject signatures_first = env->GetObjectArrayElement(signatures, 0);

    //3.5 调动signatures[0].toCharsString()
    j_clz = env->GetObjectClass(signatures_first);
    j_mid = env->GetMethodID(j_clz, "toCharsString", "()Ljava/lang/String;");
    jstring j_signature_str = (jstring)env->CallObjectMethod(signatures_first, j_mid);
    const char * signature_str = env->GetStringUTFChars(j_signature_str, NULL);
    if(strcmp(signature_str, APP_SIGNATURE) != 0){
        return;
    }
    __android_log_print(ANDROID_LOG_ERROR, "JNI_TAG", "package signature name verified.");

    //verified success
    is_verify = 1;
}

void change(int* i){
    *i = 300;
}

//题目：写个方法对两个值进行交换
void change(int* a, int* b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

//题目：写个方法对两个值进行交换，但是不开辟额外的内存。
void change2(int* a, int* b){
    *a = *a + *b;
    *b = *a - *b;
    *a = *a - *b;
}

void operate(void (method)(int, int), int num1, int num2){
    method(num1, num2);
}

void staticlloc(){

}

char* substr(char* str, int start, int end){
    int len = end - start;
    char sub[len];
    str+=start;
    for(int i = 0; i< len; i++){
        sub[i] = *str;
        str++;
    }
    sub[len] = '\0';
    return sub;
}

void lower(char* dest, char* source){
    //dest用于存放结果，大小自己指定
    while(*source != '\0'){
       char ch = *source;
       *dest = tolower(ch);
       source++;
       dest++;
    }
    *dest = '\0';
}

char* str_replace(char* str, char* src, char* dst){
    //1. 有没有 aa
    char* pos = strstr(str, src);
    if(!pos){
        return str;
    }
    //2. 计算新数组大小: 原数组大小-被替换大小+替换大小
    int newArrSize = strlen(str) - strlen(src) + strlen(dst);
    //开辟新的数组
    char result[newArrSize];

    //3. 进行拼接aabbcc bb替换fff
    int start_end_position = pos - str;
    char* start = substr(str, 0, start_end_position);
    //中间字符串fff

    //最后字符串
    char* end = substr(str, start_end_position + strlen(src), strlen(str));

    strcpy(result, start);
    strcat(result, dst);
    strcat(result, end);
    //4. 多个替换 while or 递归
    return str_replace(result, src, dst);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_testjni_VideoCompress_compressVideo(JNIEnv *env, jobject thiz, jobjectArray compress_command, jobject callback) {
    //for每一帧
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_testjni_VideoCompress_getSignature(JNIEnv *env, jobject thiz) {
    int num1 = 100;
    int num2 = 200;
    const int* number_p = &num1;
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_testjni_VideoCompress_callStaticMethod(JNIEnv *env, jclass clazz) {
    jmethodID j_mid = env->GetStaticMethodID(clazz, "getUUID", "()Ljava/lang/String;");
    jstring j_uuid = (jstring)env->CallStaticObjectMethod(clazz, j_mid);
    char* c_uuid = (char*)env->GetStringUTFChars(j_uuid, NULL);
    LOGE("uuid: %s", c_uuid);
    env->ReleaseStringUTFChars(j_uuid, c_uuid);
}

extern "C" JNIEXPORT jobject JNICALL
Java_com_example_testjni_VideoCompress_createPoint(JNIEnv *env, jclass clazz) {
    //获取Point clazz
    jclass point_clazz = env->FindClass("com/example/testjni/Point");
    jmethodID  j_mid = env->GetMethodID(point_clazz, "<init>", "(II)V");
    jobject j_point = env->NewObject(point_clazz, j_mid, 11, 22);
    return j_point;
}

class Parcel{
    char* mData;
    int mDataPos;

public:
    Parcel(){
        mData = (char *)malloc(1024);
        mDataPos = 0;
    }

    void writeInt(jint value){
        *reinterpret_cast<int*>(mData + mDataPos) = value;
        mDataPos += sizeof(int);
    }

    void setDataPosition(int pos){
        mDataPos = pos;
    }

    jint readInt(){
        jint val = *reinterpret_cast<int*>(mData + mDataPos);
        mDataPos += sizeof(int);
        return val;
    }

};

extern "C" JNIEXPORT jlong JNICALL
Java_com_example_testjni_SignatureUtils_nativeCreate(JNIEnv *env, jobject thiz) {
    Parcel* parcel = new Parcel();
    return (jlong)parcel;
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_testjni_SignatureUtils_nativeWriteInt(JNIEnv *env, jobject thiz, jlong m_native_ptr, jint val) {
    Parcel* parcel = (Parcel *)m_native_ptr;
    parcel->writeInt(val);
}

extern "C" JNIEXPORT void JNICALL Java_com_example_testjni_SignatureUtils_nativeSetDataPosition(JNIEnv *env, jobject thiz, jlong m_native_ptr, jint pos) {
    Parcel* parcel = (Parcel *)m_native_ptr;
    parcel->setDataPosition(pos);
}

extern "C" JNIEXPORT jint JNICALL Java_com_example_testjni_SignatureUtils_nativeReadInt(JNIEnv *env, jobject thiz, jlong m_native_ptr) {
    Parcel* parcel = (Parcel *)m_native_ptr;
    return parcel->readInt();
}

int comparator(const jint* n1, const jint* n2){
    return *n1 - *n2;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_testjni_Sample_sort(JNIEnv *env, jclass clazz, jintArray arr) {
    jint* intArray = env->GetIntArrayElements(arr, NULL);
    int length = env->GetArrayLength(arr);
    //第三个参数：数组元素数据类型大小
//    void qsort(void* __base, size_t __nmemb, size_t __size, int (*__comparator)(const void* __lhs, const void* __rhs));
    qsort(intArray, length, sizeof(int), reinterpret_cast<int (*)(const void *, const void *)>(comparator));

    //同步数组的书籍给java数组，intArry并不是jarray，可以简单理解为Copy
    //0: 既要同步数组给jintarry, 又要释放数组
    //#define JNI_COMMIT      1    会同步，不会释放       /* copy content, do not free buffer */
    //#define JNI_ABORT       2    不同步数据，会释放intArry       /* free buffer w/o copying back */
    env->ReleaseIntArrayElements(arr, intArray, JNI_COMMIT);
}

extern "C" JNIEXPORT void JNICALL Java_com_example_testjni_Sample_localRef(JNIEnv *env, jclass clazz) {
    //字符串截取  String对象
    jclass string_clz = env->FindClass("java/lang/String");
    jmethodID init_mid = env->GetMethodID(string_clz, "<init>", "()V");
    jobject j_str = env->NewObject(string_clz, init_mid);

    //jobject不在使用了，要回收JavaGC的源码。
    env->DeleteLocalRef(j_str);  //删除了就不能在使用了。C和C++都需要自己释放内存。静态开辟的不需要。
}

jstring globalStr;
extern "C" JNIEXPORT void JNICALL Java_com_example_testjni_Sample_createGlobalRef(JNIEnv *env, jclass clazz, jstring str) {
    //保存全局变量jstring，供其它地方使用
    // env->NewWeakGlobalRef(str); //java中的软引用
    globalStr = static_cast<jstring>(env->NewGlobalRef(str));
}

extern "C" JNIEXPORT jstring JNICALL Java_com_example_testjni_Sample_getGlobalRef(JNIEnv *env, jclass clazz) {
    return globalStr;
}

extern "C" JNIEXPORT void JNICALL Java_com_example_testjni_Sample_delGlobalRef(JNIEnv *env, jclass clazz) {
    env->DeleteGlobalRef(globalStr);
}

extern "C" JNIEXPORT void JNICALL Java_com_example_testjni_Sample_staticLocalCache(JNIEnv *env, jclass clazz, jstring name) {
    //name属性 赋值操作
    static jfieldID jfieldId = NULL;  //局部缓存，方法被多次调用，不需要反复获取jfieldId
    if(jfieldId == NULL){
        jfieldId = env->GetStaticFieldID(clazz, "name", "Ljava/lang/String;");
    } else {
        LOGE("jfieldId not null");
    }
    env->SetStaticObjectField(clazz, jfieldId, name);

}

bool compare(const Student &_Left, const Student &_Right){
    return _Left.getName() > _Right.getName();
}

class Greater2{
public:
    bool operator()(const int& num) {
        return num > 2;
    }
};

void main2(){
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_testjni_Sample_exception(JNIEnv *env, jclass clazz) {
    //没有name3在Sample
    jfieldID jfieldId =  env->GetStaticFieldID(clazz, "name", "Ljava/lang/String;");
    //异常几种处理方式
    jthrowable throwable = env->ExceptionOccurred();
    if(throwable){
//        //1.清楚异常，然后补救
//        env->ExceptionClear();
//        jfieldId =  env->GetStaticFieldID(clazz, "name", "Ljava/lang/String;");

        //2.给Java层抛异常
        env->ExceptionClear();
        // Throwe 一个java的throwable对象
        jclass no_such_clz = env->FindClass("java/lang/NoSuchFieldException");
        env->ThrowNew(no_such_clz, "NoSuchFieldException");
        return; //记得return
    }

    jstring name = env->NewStringUTF("test");
    env->SetStaticObjectField(clazz, jfieldId, name);

    main2();
}




extern "C" JNIEXPORT void JNICALL Java_com_example_testjni_Sample_arraycopy(JNIEnv *env, jclass clazz, jobject src, jint src_pos, jobject dest, jint dest_pos, jint length) {
    //static_cast/interpret_cast 都可。
    jobjectArray src_array = static_cast<jobjectArray>(src);
    jobjectArray dst_array = static_cast<jobjectArray>(dest);

    if(src_array == NULL || dst_array == NULL){
        LOGE("case not success");
        return;
    }

    for(int i = 0; i<length; i++){
        jobject obj = env->GetObjectArrayElement(src_array, i);
        env->SetObjectArrayElement(dst_array, i, obj);
    }
}