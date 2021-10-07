package com.zw.mediatools.activity

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import com.zw.mediatools.MediaTools
import com.zw.mediatools.R
import kotlinx.android.synthetic.main.activity_player.*
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch

class PlayerActivity : AppCompatActivity(), View.OnClickListener {
    private var URL = "/mnt/sdcard/aaa_test_xxx/a.mp4"
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_player)

        btn_player.setOnClickListener(this)
    }

    override fun onClick(v: View?) {
        when (v?.id) {
            R.id.btn_player -> {
                GlobalScope.launch(Dispatchers.IO) {
                    MediaTools.play(URL)
                }
            }
        }
    }
}