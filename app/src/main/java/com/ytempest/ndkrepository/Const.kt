package com.ytempest.ndkrepository

import android.content.Context
import android.os.Environment
import java.io.File

/**
 * @author qiduhe
 * @since 2021/7/27
 */
object Const {

    val dir = Environment.getExternalStorageDirectory()

    fun getOutDir(context: Context): File? {
        return context.getExternalFilesDir(Environment.DIRECTORY_PICTURES)
    }
}