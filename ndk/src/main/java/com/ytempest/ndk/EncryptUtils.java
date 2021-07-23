package com.ytempest.ndk;

/**
 * @author qiduhe
 * @since 2021/7/23
 */
public class EncryptUtils {

    static {
        System.loadLibrary("file_encrypt");
    }

    public static native void encrypt(String filePath, String outPath);

    public static native void decrypt(String filePath, String outPath);
}
