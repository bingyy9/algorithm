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
//#include "../jni/opencv2/opencv.hpp"
#include "opencv2/opencv.hpp"
#include <android/bitmap.h>
#include "BitmapMatUtil.h"
#include "cardocr.h"
#include "LinkedList.hpp"
#include "SortUtils.h"
#include "ArrayUtil.cpp"
#include <thread>
#include <mutex>
#include <future>
#include "ArrayStack.hpp"
#include "ArrayQueue.hpp"
#include "TreeNode.hpp"
#include "PriorityQueue.hpp"
#include "BSTree.hpp"
#include "AVLTree.hpp"
#include "map2.hpp"
#include "HuffmanTree.h"
#include "LeeCode.h"

using namespace std;
using namespace cv;

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
//    jstring j_command_str = (jstring)env->CallObjectMethod(signatures_first, j_mid);
//    const char * signature_str = env->GetStringUTFChars(j_signature_str, NULL);

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

void hannuota(int n, char start, char help, char end){
    if(n == 1){
        LOGE("%d from %c move to %c", n, start, end);
        return;
    }

    hannuota(n-1, start, end, help);
    LOGE("%d from %c move to %c", n, start, end);
    hannuota(n-1, help, start, end);
}

void LeeCodeTest(){
    LOGE("LeeCodeTest Native Start=====");
    int i = LeeCode::reverse(42);

    LOGE("-42/10 = %d, %d" , (-42/10) , -42%10);
    LOGE("i = ", i);
    LOGE("LeeCodeTest Native End=====");
}

void huffmanTest(){
    LOGE("huffmanTest");

    char* str = "aaaabbcccccccd";
    HuffmanTree huffmanTree;
    hTree* tree = huffmanTree.createHuffmanTree(str);
    hTable* table = huffmanTree.createHuffmanTable(tree);
    string str_encode;
    huffmanTree.encode(table, str, &str_encode);
    LOGE("str_encode: %s", str_encode.c_str());

    LeeCodeTest();
}

void mapTest(){
    LOGE("map2 test for red black tree.");
    map2<int, int> *m = new map2<int, int>();
    m->insert(3, 3);
    m->insert(2, 2);
    m->insert(1, 1);
    m->insert(4, 4);
    m->insert(5, 5);
    m->insert(-5, -5);
    m->insert(-15, -15);
    m->insert(-10, -10);
    m->insert(6, 6);
    m->insert(7, 7);

    m->layer_traverse();


    huffmanTest();
}

void AVLTreeTest(){
    LOGE("AVLTreeTest--------");
    AVLTree<int, int> *avlTree = new AVLTree<int, int>();
    avlTree->put2(3,3);
    avlTree->put2(2,2);
    avlTree->put2(1,1);
    avlTree->put2(4,4);
    avlTree->put2(5,5);
    avlTree->put2(6,6);
    avlTree->put2(7,7);
    avlTree->put2(10,10);
    avlTree->put2(9,9);
    avlTree->put2(8,8);

    avlTree->layer_traverse();

    LOGE("AVLTree remove node rotation test:");
    avlTree->remove(4);
    avlTree->layer_traverse();
    avlTree->remove(5);
    avlTree->layer_traverse();
    avlTree->remove(2);
    avlTree->layer_traverse();

    mapTest();
}

void main4(){
    BSTree<int, int> *bsTree = new BSTree<int, int>();
    //(2, -11, -13, 0, 3, 8, 7)
    bsTree->put2(2, 2);
    bsTree->put2(-11, -11);
    bsTree->put2(-13, -13);
//    bsTree->put2(0, 0);
    bsTree->put2(3, 3);
    bsTree->put2(8, 8);
    bsTree->put2(7, 7);
//    bsTree->put(2, 2);
//    bsTree->put(-11, -11);
//    bsTree->put(-13, -13);
//    bsTree->put(0, 0);
//    bsTree->put(3, 3);
//    bsTree->put(8, 8);
//    bsTree->put(7, 7);

    LOGE("layer_traverse BSTree");
    bsTree->layer_traverse(bsTree->root);

    bsTree->remove(2);
    LOGE("layer_traverse BSTree");
    bsTree->layer_traverse(bsTree->root);

    AVLTreeTest();
}

void main3(){
    TreeNode<char>* A = new TreeNode<char>('A');
    TreeNode<char>* B = new TreeNode<char>('B');
    TreeNode<char>* C = new TreeNode<char>('C');
    TreeNode<char>* D = new TreeNode<char>('D');
    TreeNode<char>* E = new TreeNode<char>('E');
    TreeNode<char>* F = new TreeNode<char>('F');
    TreeNode<char>* G = new TreeNode<char>('G');
    A->left = B;
    A->right = C;
    B->left = D;
    B->right = E;
    C->right = F;
    F->left = G;

    string str;
    TreeNode<char>::pre_order_traverse(A);
    TreeNode<char>::serialize_tree(A, &str);
    char* ch = const_cast<char *>(str.c_str());
    TreeNode<char>* node = TreeNode<char>::deserialize_tree(&ch);
    LOGE("serialize_tree %s", str.c_str());
    TreeNode<char>::pre_order_traverse(node);

    main4();
}


void main4_sort(){
    int len = 10;
    int *arr = SortUtils::create_random_array(len, 20, 100);
    int *arr2 = SortUtils::copy_rand_array(arr, len);

    LOGE("9 >> 1 %d ", 9>>1);
    SortUtils::sort_array("bubbleSort", SortUtils::quick_sort, arr, len);
//    SortUtils::sort_array("selectSort", SortUtils::selectSort, arr2, len);
    SortUtils::print_array(arr, len);
//    SortUtils::bubbleSort(arr, 6);
//    for(int i = 0; i<len; i++){
//        LOGE("%d", arr[i]);
//    }
    delete[] (arr);
    delete[] (arr2);

    main3();
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


    main4_sort();
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

void bitmap2Mat(JNIEnv *pEnv, Mat &mat, jobject bitmap) {
    //Mat 里面有个type：CV_8UC4--ARGB8888  CV_8UC2-RGB565
    AndroidBitmapInfo info;
    AndroidBitmap_getInfo(pEnv, bitmap, &info);

    //锁定Bitmap画布
    void* pixels;
    AndroidBitmap_lockPixels(pEnv, bitmap, &pixels);

    //指定mat的宽高和type BGRA
    mat.create(info.height, info.width, CV_8UC4);
    if(info.format == ANDROID_BITMAP_FORMAT_RGBA_8888){
        //CV_8UC4
        Mat temp(info.height, info.width, CV_8UC4, pixels);
        mat.create(info.height, info.width, CV_8UC4);
        temp.copyTo(mat);
    } else if(info.format == ANDROID_BITMAP_FORMAT_RGB_565){
        Mat temp(info.height, info.width, CV_8UC2, pixels);
        cvtColor(temp, mat, COLOR_BGR5652RGBA);
    } else {
        //others need to convert bitmap firstly
    }

    AndroidBitmap_unlockPixels(pEnv, bitmap);
}


void mat2Bitmap(JNIEnv *env, Mat mat, jobject &bitmap) {
    AndroidBitmapInfo  info;
    void *pixels;
    AndroidBitmap_getInfo(env, bitmap, &info);

    AndroidBitmap_lockPixels(env, bitmap, &pixels);
    if(info.format == ANDROID_BITMAP_FORMAT_RGBA_8888){
        Mat temp(info.height, info.width, CV_8UC4, pixels);
        if(mat.type() == CV_8UC4){
            mat.copyTo(temp);
        } else if(mat.type() == CV_8UC2){
            cvtColor(mat, temp, COLOR_BGR5652BGRA);
        } else if(mat.type() == CV_8UC1){
            cvtColor(mat, temp, COLOR_GRAY2RGBA);
        }
    } else if(info.format == ANDROID_BITMAP_FORMAT_RGB_565){
        Mat temp(info.height, info.width, CV_8UC2, pixels);
        if(mat.type() == CV_8UC2){
            mat.copyTo(temp);
        } else if(mat.type() == CV_8UC4){
            cvtColor(mat, temp, COLOR_BGRA2RGB);
        } else if(mat.type() == CV_8UC1){
            cvtColor(mat, temp, COLOR_GRAY2BGR565);
        }
    }
    AndroidBitmap_unlockPixels(env, bitmap);
}

CascadeClassifier cascadeClassifier;

extern "C" JNIEXPORT jint JNICALL Java_com_example_testjni_FaceDetection_faceDectionSaveInfo(JNIEnv *env, jobject thiz, jobject bitmap) {
    //检测人脸
    Mat mat;
    bitmap2Mat(env, mat, bitmap);
    //处理灰度
    Mat gray_mat;
    cvtColor(mat, gray_mat, COLOR_BGRA2GRAY);

    Mat equalize_mat;
    //直方均衡补偿
    equalizeHist(gray_mat, equalize_mat);
    //识别人脸

    std::vector<Rect> faces;
    cascadeClassifier.detectMultiScale(equalize_mat, faces, 1, 1, 5);
    LOGE("face count: %d", faces.size());
    if(faces.size() == 1){
        Rect faceRect = faces[0];

        //在人脸部位画个图
        rectangle(mat, faceRect, Scalar(255, 155, 155), 8);

        //保存人脸信息Mat，图片jpg
        Mat face_info_mat(equalize_mat, faceRect);
    }
    //把mat放入bitmap
    mat2Bitmap(env, mat, bitmap);
    //保存人脸信息
    return 0;
}


//加载人脸识别分类器文件
extern "C" JNIEXPORT void JNICALL Java_com_example_testjni_FaceDetection_loadCascade(JNIEnv *env, jobject thiz, jstring file_path) {
    const char* filePath = env->GetStringUTFChars(file_path, 0);
    cascadeClassifier.load(filePath);
    LOGE("load cascadeClassifier success");
    env->ReleaseStringUTFChars(file_path, filePath);
}

extern "C" JNIEXPORT jstring JNICALL Java_com_example_testjni_BankCardOCR_cardOcr(JNIEnv *env, jclass clazz, jobject bitmap) {
    Mat mat;
    int res = BitmapMatUtil::bitmap2mat(env, bitmap, mat);
    LOGE(" %d, %d, %d, %d", mat.cols, mat.rows, mat.type(), CV_8UC4);
    //截取银行卡区域
    Rect card_rect;
    co1::find_card_area(mat, card_rect);
    Mat card_mat(mat, card_rect);

    //截取卡号区域
    Rect card_number_rect;
    co1::find_card_number_area(card_mat, card_number_rect);
    Mat card_number_mat(card_mat, card_number_rect);

    //获取数字
    vector<Mat> numbers;
    co1::find_card_numbers(card_number_mat, numbers);

    mat2Bitmap(env, card_number_mat, bitmap);
    return env->NewStringUTF("fda");
}