package com.example.elvin.unit3;

/**
 * Created by elvin on 2017/8/28.
 */

public class TestInterface {
    static {
        System.loadLibrary("native");
    }

    public native String stringFromJNI();
    public native void showStringFromJave(String str);
}
