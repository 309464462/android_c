//
// Created by elvin on 2017/8/5.
//
#include "com_example_elvin_unit2_TestJni.h"
#include "staticlib.h"

JNIEXPORT jstring JNICALL Java_com_example_elvin_unit2_TestJni_stringFromJNI
  (JNIEnv * env, jobject thiz){
    return (*env)->NewStringUTF(env,"hello from jni module one");
}

JNIEXPORT jint JNICALL
Java_com_example_elvin_unit2_TestJni_intFromJni(JNIEnv *env, jobject instance) {
    return getNumber();

}
