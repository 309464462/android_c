package com.example.elvin.unit2;

/**
 * Created by elvin on 2017/8/5.
 */

public class TestJni {
    static {
        System.loadLibrary("NativeLib");
    }
    public native String  stringFromJNI();
    public native int intfromJNI();
}
