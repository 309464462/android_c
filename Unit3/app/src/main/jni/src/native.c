#include "com_example_elvin_unit3_TestInterface.h"
#include <stdio.h>


JNIEXPORT jstring JNICALL Java_com_example_elvin_unit3_TestInterface_stringFromJNI
  (JNIEnv * env, jobject thiz){

    return (*env)->NewStringUTF(env,"hello from JNI");
  }

JNIEXPORT void JNICALL Java_com_example_elvin_unit3_TestInterface_showStringFromJave
        (JNIEnv *env, jobject thiz, jstring javaString){
    const jbyte *str;
    jboolean isCopy;

    str = (*env)->GetStringUTFChars(env,javaString,isCopy);
    if(0 != str){
        printf("java string is:%s",str);
        if(isCopy){
            printf("java string is a copoy string of jaav string");
        }
        else{
            printf("java string is a point string of actual string");
        }
    }
}