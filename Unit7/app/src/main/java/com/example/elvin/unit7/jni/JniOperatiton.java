package com.example.elvin.unit7.jni;

/**
 * Created by elvin on 2017/8/31.
 */

public class JniOperatiton {
    static {
        System.loadLibrary("native");
    }

    public native void nativeInt();
    public native void nativeFree();
    public native void nativeWorker(int num);
}
