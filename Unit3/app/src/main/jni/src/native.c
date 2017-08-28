//
// Created by elvin on 2017/8/28.
//

#include "com_example_elvin_unit3_jni_JniTest.h"

#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}

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

JNIEXPORT jint JNICALL Java_com_example_elvin_unit3_jni_JniTest_sumNumber
        (JNIEnv *env, jobject thiz, jintArray intArray){
    jint nativeInt[10];
    jint result = 0;
    (*env)->GetIntArrayRegion(env,intArray,0,10,nativeInt);
    for(int i = 0 ; i < 10 ; i++){
        result = result + nativeInt[i];
        nativeInt[i] = nativeInt[i] + 5;
    }

    (*env)->SetIntArrayRegion(env,intArray,0,10,nativeInt);
    return result;
}

JNIEXPORT jboolean JNICALL Java_com_example_elvin_unit3_jni_JniTest_controlPointOfArray
        (JNIEnv *env, jobject thiz, jintArray intArray,jint size){
    jint * nativeDirectArray;
    jboolean isCopy;
    nativeDirectArray = (*env)->GetIntArrayElements(env,intArray,&isCopy);
    //指针是无法获取指向的内存空间内容的大小的
    for(int i = 0 ; i < size; i++){
        nativeDirectArray[i] =nativeDirectArray[i] + 5;
    }
    (*env)->ReleaseIntArrayElements(env,intArray,nativeDirectArray,0);
    return isCopy;
}
