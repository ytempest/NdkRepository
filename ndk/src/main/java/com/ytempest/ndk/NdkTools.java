package com.ytempest.ndk;

/**
 * @author qiduhe
 * @since 2021/7/22
 */
public class NdkTools {

    static {
        System.loadLibrary("ndk_tools");
    }

    public static native String getText();
}
