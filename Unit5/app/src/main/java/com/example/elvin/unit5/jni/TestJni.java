package com.example.elvin.unit5.jni;

/**
 * Created by elvin on 2017/8/29.
 */

public class TestJni {
    static{
        System.loadLibrary("native");
    }

    public native void startToShowLog();
}
