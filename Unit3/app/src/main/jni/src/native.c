//
// Created by elvin on 2017/8/28.
//

#include "com_example_elvin_unit3_jni_JniTest.h"

JNIEXPORT jstring JNICALL Java_com_example_elvin_unit3_jni_JniTest_strFromJni
  (JNIEnv *env ,jobject thiz, jstring javastring){
  return (*env)->NewStringUTF(env,"asdf");
  }