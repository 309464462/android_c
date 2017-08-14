package com.example.elvin.unit2;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        TextView textView = (TextView) findViewById(R.id.tv);
        textView.setText(helloFromFFmpeg());
    }
    public native String helloFromFFmpeg();
    static{
        System.loadLibrary("live_jni");
    }
}
