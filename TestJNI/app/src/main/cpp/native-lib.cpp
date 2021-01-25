#include <jni.h>
#include <string>
#include "md5.h"

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

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_testjni_SignatureUtils_signatureParams(JNIEnv *env, jclass clazz, jstring params_) {
    // TODO: implement signatureParams()
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