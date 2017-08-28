//
// Created by elvin on 2017/8/28.
//

#include "com_example_elvin_unit3_jni_JniTest.h"

JNIEXPORT jstring JNICALL Java_com_example_elvin_unit3_jni_JniTest_strFromJni
          (JNIEnv *env ,jobject thiz, jstring javastring){
//            return (*env)->NewString(env,"asdf",4);  //这是unicode
        return (*env)->NewStringUTF(env,"asdf");  //这是utf
  }

JNIEXPORT jboolean JNICALL Java_com_example_elvin_unit3_jni_JniTest_testString
        (JNIEnv *env ,jobject thiz,jstring javaString){
       const char * str;
       jboolean  isCopy;

       str = (*env)->GetStringUTFChars(env,javaString,&isCopy);
      (*env)->ReleaseStringUTFChars(env,javaString,str);
       return isCopy;
}