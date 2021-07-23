#include <jni.h>
#include <string>
#include <cstdlib>
#include <android/log.h>

#define LOGI(CONTENT, ...) __android_log_print(ANDROID_LOG_INFO,"file_encrypt.cpp",CONTENT,__VA_ARGS__)

char secret_key[15] = "iamytempest";

void xor_per_char(const char *in_path, const char *out_path) {
    // 打开文件
    FILE *in_file = fopen(in_path, "rb");
    FILE *out_file = fopen(out_path, "wb");

    // 获取密码长度
    int key_len = strlen(secret_key);

    // 每读取一个字符就跟密码进行异或处理，然后将异或后的字符写到加密文件中
    int ch;
    int i = 0;
    while ((ch = fgetc(in_file)) != EOF) {
        ch = ch ^ secret_key[i % key_len];
        fputc(ch, out_file);
        i++;
    }

    // 关闭文件
    fclose(in_file);
    fclose(out_file);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_ytempest_ndk_EncryptUtils_encrypt(JNIEnv *env, jclass clazz, jstring file_path,
                                           jstring target_path) {
    // 将Java字符串转换成C中的字符串
    const char *in_path = (*env).GetStringUTFChars(file_path, JNI_FALSE);
    const char *out_path = (*env).GetStringUTFChars(target_path, JNI_FALSE);

    xor_per_char(in_path, out_path);

    // 释放内存
    (*env).ReleaseStringUTFChars(file_path, in_path);
    (*env).ReleaseStringUTFChars(target_path, out_path);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ytempest_ndk_EncryptUtils_decrypt(JNIEnv *env, jclass clazz, jstring file_path,
                                           jstring target_path) {
    // 将Java字符串转换成C中的字符串
    const char *in_path = (*env).GetStringUTFChars(file_path, JNI_FALSE);
    const char *out_path = (*env).GetStringUTFChars(target_path, JNI_FALSE);

    xor_per_char(in_path, out_path);

    // 释放内存
    (*env).ReleaseStringUTFChars(file_path, in_path);
    (*env).ReleaseStringUTFChars(target_path, out_path);
}

