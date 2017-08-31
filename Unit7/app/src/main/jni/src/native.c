//
// Created by elvin on 2017/8/31.
//

#include "com_example_elvin_unit7_jni_JniOperatiton.h"
#include "my_log.h"
#include  <stdio.h>
#include  <stdlib.h>
#include <unistd.h>

static jmethodID gOnNativeMesssage = NULL;

/*
 * Class:     com_example_elvin_unit7_jni_JniOperatiton
 * Method:    nativeInt
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_example_elvin_unit7_jni_JniOperatiton_nativeInt
  (JNIEnv *env, jobject obj){
      if(NULL == gOnNativeMesssage){
         jclass  clazz = (*env)->GetObjectClass(env,obj);
          //参数的括号很重要
        gOnNativeMesssage = (*env)->GetMethodID(env,clazz,"onNativeMessage","(Ljava/lang/String;)V");

        //抛出异常
        if(NULL == gOnNativeMesssage){
            MY_LOG_INFO("no find the method.................");
            //寻找类，可以现在代码中查询确定
          jclass  execptionClazz = (*env)->FindClass(env,"java/lang/RuntimeException");
          (*env)->ThrowNew(env,execptionClazz,"Unable to find method");
        }else{
          MY_LOG_INFO("find method.................");
        }
      }
  }

/*
 * Class:     com_example_elvin_unit7_jni_JniOperatiton
 * Method:    nativeFree
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_example_elvin_unit7_jni_JniOperatiton_nativeFree
  (JNIEnv *env, jobject obj){
  }


/*
 * Class:     com_example_elvin_unit7_jni_JniOperatiton
 * Method:    nativeWorker
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_example_elvin_unit7_jni_JniOperatiton_nativeWorker
  (JNIEnv *env, jobject obj, jint id,jint iterrations){
        //循环给定的次数
     for(int i = 0; i < iterrations;i++){
         //准备消息
         char message[26];
         sprintf(message,"Worker %d: Iteration %d",id,iterrations);

         //来自c字符串的消息
         jstring  messageString = (*env)->NewStringUTF(env,message);

         //调用原生消息方法
         (*env)->CallVoidMethod(env,obj,gOnNativeMesssage,messageString);
         //检查是否产生异常
         if(NULL != (*env)->ExceptionOccurred(env)){
             break;
         }
         //睡眠一秒钟
         sleep(1);
     }
  }