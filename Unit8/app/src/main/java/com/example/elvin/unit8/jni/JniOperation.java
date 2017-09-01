package com.example.elvin.unit8.jni;

/**
 * Created by elvin on 2017/9/1.
 */

public class JniOperation {
    static {
        System.loadLibrary("native");
    }

    public native void test();
}
