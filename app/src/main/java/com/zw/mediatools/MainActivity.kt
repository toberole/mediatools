package com.zw.mediatools

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import androidx.core.app.ActivityCompat
import com.zw.mediatools.activity.CMDActivity
import com.zw.mediatools.activity.PlayerActivity
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity(), View.OnClickListener {
    private var PS = arrayOf<String>(
        android.Manifest.permission.READ_EXTERNAL_STORAGE,
        android.Manifest.permission.WRITE_EXTERNAL_STORAGE,
        android.Manifest.permission.RECORD_AUDIO,
        android.Manifest.permission.INTERNET,
        android.Manifest.permission.ACCESS_NETWORK_STATE,
    )

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        ActivityCompat.requestPermissions(this, PS, 110)
        btn_cmd.setOnClickListener(this)
        btn_player.setOnClickListener(this)


    }

    override fun onClick(v: View?) {
        when (v?.id) {
            R.id.btn_cmd -> {
                var intent = Intent(this@MainActivity, CMDActivity::class.java)
                startActivity(intent)
            }

            R.id.btn_player -> {
                var intent = Intent(this@MainActivity, PlayerActivity::class.java)
                startActivity(intent)
            }
        }
    }
}