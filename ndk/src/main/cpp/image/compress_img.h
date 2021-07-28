
#ifndef NDKREPOSITORY_COMPRESS_IMG_H
#define NDKREPOSITORY_COMPRESS_IMG_H

extern "C"
JNIEXPORT jint JNICALL
Java_com_ytempest_ndk_ImageUtils_compressBitmap(JNIEnv *env, jclass clazz, jobject bitmap,
                                                jstring dest_path, jint quality);

#endif //NDKREPOSITORY_COMPRESS_IMG_H
