package com.ytempest.ndkrepository

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.ytempest.ndk.NdkTools
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Example of a call to a native method
        sample_text.text = NdkTools.getText()
    }
}
