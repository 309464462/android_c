package com.example.elvin.unit3;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.example.elvin.unit3.jni.JniTest;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;

public class MainActivity extends AppCompatActivity {


    @BindView(R.id.tvTips)
    TextView tvTips;

    private JniTest jni;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ButterKnife.bind(this);
        initview();
    }

    private void initview(){
        jni = new JniTest();
        boolean is = jni.testString("test");
        if(is){
            Log.d("jni","is copy to jni");
        }else{
            Log.d("jni"," acutual point to jni");
        }
    }

    @OnClick(R.id.tvTips)
    public void OnClick(View view){
        tvTips.setText(jni.strFromJni(""));
    }


}
