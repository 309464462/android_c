//
// Created by elvin on 2017/8/31.
//

#include "com_example_elvin_unit7_jni_JniOperatiton.h"
#include "my_log.h"
#include  <stdio.h>
#include  <stdlib.h>
#include <unistd.h>
#include <pthread.h>

static jmethodID gOnNativeMesssage = NULL;
#define FUN(A) Java_com_example_elvin_unit7_jni_JniOperatiton_## A
//原生worker的参数
struct NativeWorkerArgs{
    jint id;
    jint iterations;
};
//java 虚拟机接口指针
static JavaVM  * gVm = NULL;

//对象的全局引用
static jobject gObj = NULL;

static JNIEnv *gEnv = NULL;

static pthread_mutex_t mutex;

/*
 * Class:     com_example_elvin_unit7_jni_JniOperatiton
 * Method:    nativeInt
 * Signature: ()V
 */
JNIEXPORT void JNICALL FUN(nativeInt)(JNIEnv *env, jobject obj){

    //初始化互斥量
    if (0 != pthread_mutex_init(&mutex, NULL)) {
        //异常
        jclass exceptionClazz = (*env)->FindClass(env,"java/lang/RuntimeException");
        //抛出
        (*env)->ThrowNew(env,exceptionClazz, "Unable to init mutex--");
    }
        //如果对象的全局引用没有设置
        if(NULL == gObj){
            gObj = (*env)->NewGlobalRef(env,obj);
            if(NULL == gObj){
                goto exit;
            }
        }

        if(NULL == gEnv){
            gEnv = env;
        }
        //

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
        exit:
             return;
  }

/*
 * Class:     com_example_elvin_unit7_jni_JniOperatiton
 * Method:    nativeFree
 * Signature: ()V
 */
JNIEXPORT void JNICALL FUN(nativeFree)(JNIEnv *env, jobject obj){
    if(NULL != gObj){
        (*env)->DeleteGlobalRef(env,gObj);
        gObj = NULL;
    }

    //释放互斥量
    if (0 != pthread_mutex_destroy(&mutex)) {
        //异常
        jclass exceptionClazz = (*env)->FindClass(env,"java/lang/RuntimeException");
        //抛出
        (*env)->ThrowNew(env,exceptionClazz, "Unable to destroy mutex--");
    }

  }


/*
 * Class:     com_example_elvin_unit7_jni_JniOperatiton
 * Method:    nativeWorker
 * Signature: (I)V
 */
JNIEXPORT void JNICALL FUN(nativeWorker)(JNIEnv *env, jobject obj, jint id,jint iterrations){

    //lock
    if (0 != pthread_mutex_lock(&mutex)) {
        //异常
        jclass exceptionClazz = (*env)->FindClass(env,"java/lang/RuntimeException");
        //抛出
        (*env)->ThrowNew(env,exceptionClazz, "Unable to lock mutex--");
        return;
    }

    //循环给定的次数
     for(int i = 0; i < iterrations;i++){
         //准备消息
         char message[26];
         sprintf(message,"nativeWorker Worker %d: Iteration %d",id,i);

         //来自c字符串的消息
         jstring  messageString = (*env)->NewStringUTF(env,message);

         //调用原生消息方法
         (*env)->CallVoidMethod(env,obj,gOnNativeMesssage,messageString);
         //检查是否产生异常
         if(NULL != (*env)->ExceptionOccurred(env)){
             break;
         }
         //睡眠一秒钟,交出cpu控制权，才会出现轮询效果
         sleep(1);

     }

    //unlock
    if (0 != pthread_mutex_unlock(&mutex)) {
        //异常
        jclass exceptionClazz = (*env)->FindClass(env,"java/lang/RuntimeException");
        //抛出
        (*env)->ThrowNew(env,exceptionClazz, "Unable to unlock mutex--");
    }
  }

/**
 * 模拟原生线程的工作内容 *
 * */

static void * nativeWorkerThread(void *args){
     JNIEnv *env = NULL;
    //将当前线程附加到Java虚拟机上
    //并且获得JNIEnv接口指针
    jint result = (*gVm)->AttachCurrentThread(gVm,&env,NULL);
    if(0 == result){
        //获取原生线程 woker 线程参数
        struct NativeWorkerArgs * nativeWorkerArgs = (struct NativeWorkerArgs *) args;
        //线程上下文中运行原生worker
        JNICALL FUN(nativeWorker)(env,gObj,nativeWorkerArgs->id,nativeWorkerArgs->iterations);
        //释放原生worker线程参数
        free(nativeWorkerArgs);
        //从 Java 虚拟机中分离当前线程
        (*gVm)->DetachCurrentThread(gVm);
    }

    return (void*)1;
}

//loadLibrary的时候自动调用,在这里获得全局vm引用
jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    gVm = vm;
    MY_LOG_DEBUG("JNI_OnLoad");
    return JNI_VERSION_1_6;
}

/*
 * 由于posix线程不是java平台的一部分，因此虚拟机不能识别它们，为了和java空间交互，posix线程应该先把自己依附到虚拟机上。
 * Class:     com_example_elvin_unit7_jni_JniOperatiton
 * Method:    posixThreads
 * Signature: (II)V
 */
JNIEXPORT void JNICALL FUN(posixThreads)(JNIEnv *env, jobject obj, jint threads, jint iteration){
    //为每一个worker创建一个posix线程
    //获取每个原生线程的句柄
    pthread_t* handles =malloc(sizeof(pthread_t)*threads);
    for(jint i = 0 ; i < threads;i++){
        //原生 worker 线程参数
        struct NativeWorkerArgs * nativeWorkerArgs = malloc(sizeof(struct NativeWorkerArgs));
        nativeWorkerArgs->id = i;
        nativeWorkerArgs->iterations = iteration;
        //创建一个新线程
        int result= pthread_create(&handles[i],NULL,nativeWorkerThread,(void *)nativeWorkerArgs);
        if(0 != result){
            //捕获异常
            jclass  execptionClazz = (*env)->FindClass(env,"java/lang/RuntimeException");
            (*env)->ThrowNew(env,execptionClazz,"Unable to create thread");
            goto exit;
        }
    }
    //等待线程终止
//    for(int i = 0 ; i < threads ; i++){
//        void * result = NULL;
//
//        //它将挂起目标线程的执行，知道目标线程终止
//        if(0 !=  pthread_join(handles[i],&result)){
//            //获取异常类
//            jclass  execptionClazz = (*env)->FindClass(env,"java/lang/RuntimeException");
//            (*env)->ThrowNew(env,execptionClazz,"Unable to join thread");
//        }else{
//            //准备message
//            char message[26];
//            sprintf(message,"thread join Worker %d returned %d",i,result);
//            //来自c字符创的message
//            jstring meesageString = (*env)->NewStringUTF(env,message);
//            //调用原生消息
//            //调用原生消息方法
//            (*env)->CallVoidMethod(env,obj,gOnNativeMesssage,meesageString);
//            //检查是否产生异常
//            if(NULL != (*env)->ExceptionOccurred(env)){
//               goto  exit;
//            }
//        }
//    }
    exit:
        free(handles);
        return;
}
