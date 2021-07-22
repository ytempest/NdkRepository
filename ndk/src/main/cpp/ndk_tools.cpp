#include <jni.h>
#include <string>

extern "C"

JNIEXPORT jstring JNICALL
Java_com_ytempest_ndk_NdkTools_getText(JNIEnv *env, jclass clazz) {
    std::string hello = "Hello ytempest";
    return env->NewStringUTF(hello.c_str());
}
