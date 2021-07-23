package com.ytempest.ndkrepository

import android.Manifest
import android.os.Bundle
import android.os.Environment
import android.util.Log
import android.view.View
import android.widget.LinearLayout
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.view.forEach
import com.ytempest.ndk.EncryptUtils
import java.io.File

class FileEncryptActivity : AppCompatActivity(), View.OnClickListener {

    private val TAG = "FileEncryptActivity"

    private val dir = Environment.getExternalStorageDirectory()
    private val srcFile = File(dir, "src_img.png")

    private val targetDir by lazy { getExternalFilesDir(Environment.DIRECTORY_PICTURES) }
    private val encryptFile by lazy { File(targetDir, "encrypt_src_img.png") }
    private val decryptFile by lazy { File(targetDir, "decrypt_src_img.png") }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_file_encrypt)

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
            R.id.file_encrypt -> {
                Thread() {
                    Log.d(TAG, "onClick: start encrypt: ")
                    Log.d(TAG, "onClick: path: " + encryptFile.absolutePath)
                    EncryptUtils.encrypt(srcFile.absolutePath, encryptFile.absolutePath)
                    Log.d(TAG, "onClick: end encrypt: ")
                }.start()
            }
            R.id.file_decrypt -> {
                Thread() {
                    Log.d(TAG, "onClick: start decrypt: ")
                    Log.d(TAG, "onClick: path: " + decryptFile.absolutePath)
                    EncryptUtils.decrypt(encryptFile.absolutePath, decryptFile.absolutePath)
                    Log.d(TAG, "onClick: end decrypt: ")
                }.start()
            }
            R.id.file_clear -> {
                val success = encryptFile.delete() || decryptFile.delete()
                Log.d(TAG, "onClick: delete success: $success")
            }
        }
    }
}