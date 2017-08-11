package com.example.elvin.unit2;

/**
 * Created by elvin on 2017/8/11.
 */

public class TestJni2 {
    static{
        System.loadLibrary("NativeLib2");
    }
    public native String  stringFromJNI();
}
