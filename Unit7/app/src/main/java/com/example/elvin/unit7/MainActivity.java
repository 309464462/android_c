package com.example.elvin.unit7;

import android.os.Bundle;
import android.support.annotation.IntegerRes;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.TextView;

import com.example.elvin.unit7.UIinterface.Ilogshwo;
import com.example.elvin.unit7.jni.JniOperatiton;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;

public class MainActivity extends AppCompatActivity {

    @BindView(R.id.etThread)
    EditText etThread;
    @BindView(R.id.etIteration)
    EditText etIteration;
    @BindView(R.id.tvClick)
    Button tvClick;
    @BindView(R.id.log_view)
    TextView logView;
    @BindView(R.id.scrollView)
    ScrollView scrollView;

    private JniOperatiton jni;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ButterKnife.bind(this);
        initdata();
    }

    private void initdata() {
        jni = new JniOperatiton(logs);
        //初始化原生代码
        jni.nativeInt();

    }

    private Ilogshwo logs = new Ilogshwo(){

        @Override
        public void refreshlog(final String str) {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    logView.append(str);
                    logView.append("\n");
                }
            });
        }
    };

    @Override
    protected void onDestroy() {
        //释放原生资源
        jni.nativeFree();
        super.onDestroy();
    }



    @OnClick(R.id.tvClick)
    public void OnClick(View view) {
        int id = view.getId();
        switch (id) {
            case R.id.tvClick:
                int threads = getNumber(etThread,0);
                int iteration = getNumber(etIteration,0);
                if(threads > 0 && iteration > 0){
                    startThreads(threads,iteration);
                }
                break;
            default:
                break;
        }
    }

    /*
    * 在原生代码中利用多线程的好处的最简单方法就是使用Java线程。可以在java空间用纯Java代码创建java.lang.Thread 实例，并在其上下文中调用方法，
    * 这种方法的主要优点是不要原生代码做任何修改
    * */
    private void startThreads(int threads,final  int iteration) {
        //在java代码中创建线程，那么原生代码就不用发生改变
//        javaThread(threads,iteration);
        //在原生代码中创建线程
        jni.posixThreads(threads,iteration);
    }

    private void javaThread(int threads,final int iterations){
        for(int i =0;i < threads;i++){
            final int id =i;
            Thread thread = new Thread(){
                @Override
                public void run() {
                    jni.nativeWorker(id,iterations);
                }
            };
            thread.start();
        }
    }



    public int getNumber(EditText et,int defaultNumber){
        int value;
        try{
            value = Integer.parseInt(et.getText().toString());
        }catch (NumberFormatException e){
            value = defaultNumber;
        }
        return value;
    }
}
