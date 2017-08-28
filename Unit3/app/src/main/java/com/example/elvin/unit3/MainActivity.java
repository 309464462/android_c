package com.example.elvin.unit3;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    private  TestInterface in = new TestInterface();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initdata();
    }

    public void initdata(){
        in.showStringFromJave("copy value");


    }

    public void OnClick(View view){
        ((TextView)view).setText(in.stringFromJNI());
    }
}
