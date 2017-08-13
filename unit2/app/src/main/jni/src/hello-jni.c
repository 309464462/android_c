//
// Created by elvin on 2017/8/5.
//
#include "com_example_elvin_unit2_TestJni.h"

JNIEXPORT jstring JNICALL Java_com_example_elvin_unit2_TestJni_stringFromJNI
  (JNIEnv * env, jobject thiz){
    return (*env)->NewStringUTF(env,"hello from jni module extramodule 2");
}
JNIEXPORT jint JNICALL
Java_com_example_elvin_unit2_TestJni_intfromJNI(JNIEnv *env, jobject instance) {
 return 0;

}
