package com.example.elvin.unit3.jni;

/**
 * Created by elvin on 2017/8/28.
 */

public class JniTest {
    static{
        System.loadLibrary("native");
    }

    //
    private String instanceFiled = "Instance Filed";
    private static String staticFiled = "Static Filed";

    private String instanceMethod(){
        return "Instance Method";
    }

    private static String staticMethod(){
        return "Static Method";
    }

    public native String strFromJni(String str);
    public native  boolean testString(String  str);
    public native  int  sumNumber(int[] nums);
    public native  boolean controlPointOfArray(int[] nums,int length);

    public native String getFiledFromJava();


}
