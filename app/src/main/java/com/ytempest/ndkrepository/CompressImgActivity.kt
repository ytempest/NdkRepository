package com.ytempest.ndkrepository

import android.Manifest
import android.graphics.BitmapFactory
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.LinearLayout
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.view.forEach
import com.ytempest.ndk.ImageUtils
import java.io.File

class CompressImgActivity : AppCompatActivity(), View.OnClickListener {

    private val TAG = "CompressImgActivity"

    private val srcImage = File(Const.dir, "src_raw_image.png")
    private val compressImage by lazy { File(Const.getOutDir(this), "compress_image.png") }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_comperss_img)

        val container = findViewById<LinearLayout>(R.id.container)
        container.forEach {
            it.setOnClickListener(this)
        }

        ActivityCompat.requestPermissions(
            this,
            arrayOf(Manifest.permission.WRITE_EXTERNAL_STORAGE),
            11
        )
    }

    override fun onClick(v: View?) {
        when (v?.id) {
            R.id.compress_img -> {
                compress(srcImage, compressImage, 80)
            }
        }
    }

    private fun compress(srcFile: File, destFile: File, quality: Int) {
        Thread() {
            Log.d(TAG, "onClick: start compress: ")
            Log.d(TAG, "onClick: src path: " + srcFile.absolutePath)
            Log.d(TAG, "onClick: compress path: " + destFile.absolutePath)
            val result = ImageUtils.compressBitmap(
                BitmapFactory.decodeFile(srcFile.absolutePath),
                destFile.absolutePath,
                quality
            )
            Log.d(TAG, "onClick: end compress: result=${result} ")
        }.start()
    }
}