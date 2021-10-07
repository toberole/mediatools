package com.zw.mediatools.activity

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import com.zw.mediatools.R
import kotlinx.android.synthetic.main.activity_cmd.*

class CMDActivity : AppCompatActivity(), View.OnClickListener {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_cmd)
        btn_test1.setOnClickListener(this)
    }

    override fun onClick(v: View?) {
        when (v?.id) {
            R.id.btn_test1 -> {

            }
        }
    }
}