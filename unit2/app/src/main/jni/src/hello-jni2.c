//
// Created by elvin on 2017/8/5.
//
#include "com_example_elvin_unit2_TestJni2.h"

JNIEXPORT jstring JNICALL Java_com_example_elvin_unit2_TestJni2_stringFromJNI
        (JNIEnv * env, jobject thiz){
    return (*env)->NewStringUTF(env,"hello from jni module two");
}