#include <jni.h>
#include <string>
#include <android/bitmap.h>
#include <android/log.h>
#include <cstdio>
#include <csetjmp>
#include <cmath>
#include <cstdint>
#include <ctime>

// 统一编译方式
extern "C" {
#include "compress_img.h"
#include "jpeg/jpeglib.h"
#include "jpeg/jconfig.h"
#include "jpeg/jerror.h"
#include "jpeg/jmorecfg.h"
}

// log打印
#define LOG_TAG "compress_img"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

#define SUCCESS_COMPRESS  1
#define ERR_UNKNOWN  -1
#define ERR_UN_SUPPORT_FORMAT  -2
#define ERR_OPEN_OUT_FILE  -3

// error 结构体
struct error_mgr {
    struct jpeg_error_mgr jpeg_error;
    jmp_buf setjmp_buffer;
};
typedef error_mgr *error_ptr;
typedef uint8_t BYTE;

static void error_exit(j_common_ptr cinfo) {
    // cinfo->err really points to a my_error_mgr struct, so coerce pointer
    error_ptr err = (error_ptr) cinfo->err;

    // Always display the message.
    // We could postpone this until after returning, if we chose.
    (*cinfo->err->output_message)(cinfo);

    jpeg_error_mgr pub = err->jpeg_error;
    LOGI("jpeg_message_table[%d]: %s", pub.msg_code, pub.jpeg_message_table[pub.msg_code]);

    // Return control to the setjmp point
    longjmp(err->setjmp_buffer, 1);
}

int generateJPEG(BYTE *data, int width, int height, int quality, const char *out_path,
                 jboolean optimize) {
    struct jpeg_compress_struct cinfo;

    //当读完整个文件的时候就会回调my_error_exit这个退出方法。
    struct error_mgr err_mgr;

    jpeg_error_mgr &mgr = err_mgr.jpeg_error;
    cinfo.err = jpeg_std_error(&mgr);
    mgr.error_exit = error_exit;
    // setjmp是一个系统级函数，是一个回调。
    if (setjmp(err_mgr.setjmp_buffer)) {
        return ERR_UNKNOWN;
    }

    // 初始化结构体
    jpeg_create_compress(&cinfo);
    // 打开输出文件
    FILE *file = fopen(out_path, "wb");
    if (file == nullptr) {
        return ERR_OPEN_OUT_FILE;
    }

    // 设置结构体文件路径，以及宽高
    jpeg_stdio_dest(&cinfo, file);
    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.arith_code = TRUE; // TRUE=arithmetic coding, FALSE=Huffman
    cinfo.input_components = 3; // 颜色的组成 rgb，三个 # of color components in input image
    cinfo.in_color_space = JCS_RGB; // 设置颜色空间为RGB

    jpeg_set_defaults(&cinfo);

    cinfo.optimize_coding = optimize; // 是否采用哈弗曼
    jpeg_set_quality(&cinfo, quality, TRUE); // 设置质量

    // 开始压缩
    jpeg_start_compress(&cinfo, TRUE);

    JSAMPROW row_pointer[1];
    int row_stride = cinfo.image_width * cinfo.input_components;
    while (cinfo.next_scanline < cinfo.image_height) {
        // 得到一行的首地址
        row_pointer[0] = &data[cinfo.next_scanline * row_stride];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    // 结束压缩
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    // 关闭文件
    fclose(file);

    return SUCCESS_COMPRESS;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_ytempest_ndk_ImageUtils_compressBitmap(JNIEnv *env, jclass clazz, jobject bitmap,
                                                jstring dest_path, jint quality) {
    // 1、解析Bitmap
    AndroidBitmapInfo info;
    AndroidBitmap_getInfo(env, bitmap, &info);

    int bitmap_width = info.width;
    int bitmap_height = info.height;
    int bitmap_format = info.format;

    LOGI("bitmap width:%d,  height:%d, format:%d", bitmap_width, bitmap_height, bitmap_format);

    // 目前仅支持ARGB8888
    if (bitmap_format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGI("un support bitmap format : %d", bitmap_format);
        return ERR_UN_SUPPORT_FORMAT;
    }

    // 2、解析数据

    // 将bitmap解析到数组中，数组中保存的是rgb，然后转成 YCbCr
    BYTE *pixel_color;
    // 锁定画布
    AndroidBitmap_lockPixels(env, bitmap, (void **) &pixel_color);

    // 申请一块内存，大小 = 宽 * 高 * 3
    BYTE *data = (BYTE *) malloc(bitmap_width * bitmap_height * 3);
    // 新建指针指向数组，原数组指针用户释放内存
    BYTE *data_point = data;

    // 一个一个像素解析保存到data
    BYTE r, g, b;
    int color;
    for (int row = 0; row < bitmap_height; ++row) {
        for (int col = 0; col < bitmap_width; ++col) {
            // 获取二位数组的每一个像素信息的首地址
            color = *((int *) pixel_color);
            // 把rgb取出来
            r = ((color & 0x00FF0000) >> 16);
            g = ((color & 0x0000FF00) >> 8);
            b = ((color & 0x000000FF));

            // 保存到data里面去
            *data_point = b;
            *(data_point + 1) = g;
            *(data_point + 2) = r;

            // 移动指针
            data_point += 3;
            // 一个像素点保存argb四个值
            pixel_color += 4;
        }
    }

    // 解锁画布
    AndroidBitmap_unlockPixels(env, bitmap);

    // 3、压缩图片

    char *out_path = (char *) env->GetStringUTFChars(dest_path, FALSE);

    int result = generateJPEG(data, bitmap_width, bitmap_height, quality, out_path, true);
    LOGI("compress: %d, out path: %s", result, out_path);

    // 4、释放内存
    free(data);
    env->ReleaseStringUTFChars(dest_path, out_path);
    jclass bitmap_class = env->GetObjectClass(bitmap);
    jmethodID recycle_method_id = env->GetMethodID(bitmap_class, "recycle", "()V");
    env->CallVoidMethod(bitmap, recycle_method_id);

    return result;
}
