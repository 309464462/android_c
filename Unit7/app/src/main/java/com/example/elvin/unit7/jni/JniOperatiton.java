package com.example.elvin.unit7.jni;

import android.app.Activity;

import com.example.elvin.unit7.MainActivity;
import com.example.elvin.unit7.UIinterface.Ilogshwo;


/**
 * Created by elvin on 2017/8/31.
 */

public class JniOperatiton {
    static {
        System.loadLibrary("native");
    }

    private Ilogshwo loglisten;

    public JniOperatiton(Ilogshwo logs) {
        loglisten = logs;
    }

    /*
   * 这个接口由原生代码调用
   * */
    private void onNativeMessage(String message){
        loglisten.refreshlog(message);
    }

    public native void nativeInt();
    public native void nativeFree();
    public native void nativeWorker(int id,int iterrations);
}
