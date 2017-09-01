#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "com_example_elvin_unit7_jni_JniOperatiton.h"
#include "my_log.h"

#define LOG_TAG "LOG FROM JNI"

#define LOGW(a)  __android_log_write(ANDROID_LOG_WARN,LOG_TAG,a)

//传递pthread参数用的结构体
struct NativeWorkerArgs {
    jint id;
    jint iterations;
};

//回调java的方法
static jmethodID gOnNativeMessage = NULL;
static JavaVM* gVm = NULL; //虚拟机引用，作为全局变量
static jobject gObj = NULL;
static pthread_mutex_t mutex;

#define FUN(A) Java_com_example_elvin_unit7_jni_JniOperatiton_## A

//loadLibrary的时候自动调用,在这里获得全局vm引用
jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    gVm = vm;

    LOGW("JNI_OnLoad");
    return JNI_VERSION_1_4;
}

void  FUN(nativeInt)(JNIEnv *env, jobject obj) {

    //初始化互斥量
    if (0 != pthread_mutex_init(&mutex, NULL)) {
        //异常
        jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
        //抛出
        env->ThrowNew(exceptionClazz, "Unable to init mutex--");
    }

    if (NULL == gObj) {
        gObj = env->NewGlobalRef(obj);
    }

    //初始java回调
    if (NULL == gOnNativeMessage) {
        jclass clazz = env->GetObjectClass(obj);
        gOnNativeMessage = env->GetMethodID(clazz, "onNativeMessage",
                                            "(Ljava/lang/String;)V");

        if (NULL == gOnNativeMessage) {
            //异常
            jclass exceptionClazz = env->FindClass(
                    "java/lang/RuntimeException");
            //抛出
            env->ThrowNew(exceptionClazz, "Unable to find method--");
        }
    }

}

void  FUN(nativeFree)(JNIEnv *env, jobject) {

    //释放全局变量
    if (NULL != gObj) {
        env->DeleteGlobalRef(gObj);
        gObj = NULL;
    }

    //释放互斥量
    if (0 != pthread_mutex_destroy(&mutex)) {
        //异常
        jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
        //抛出
        env->ThrowNew(exceptionClazz, "Unable to destroy mutex--");
    }
}

//ndk线程执行的代码
void nativeWorker(JNIEnv *env, jobject obj, jint id, jint iterations) {

    //lock
    if (0 != pthread_mutex_lock(&mutex)) {
        //异常
        jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
        //抛出
        env->ThrowNew(exceptionClazz, "Unable to lock mutex--");
        return;
    }

    for (jint i = 0; i < iterations; i++) {
        char message[26];
        sprintf(message, "Worker %d:Iteration %d", id, i);

        //回调java方法
        jstring messageString = env->NewStringUTF(message);
        env->CallVoidMethod(obj, gOnNativeMessage, messageString);

        if (NULL != env->ExceptionOccurred()) {
            break;
        }
        sleep(1);
    }

    //unlock
    if (0 != pthread_mutex_unlock(&mutex)) {
        //异常
        jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
        //抛出
        env->ThrowNew(exceptionClazz, "Unable to unlock mutex--");

    }
}

void  FUN(nativeWorker)(JNIEnv *env, jobject obj,jint id, jint iterations) {
    nativeWorker(env, obj, id, iterations);
}

//pthread执行的方法
static void* nativeWorkerThread(void* args) {
    JNIEnv* env = NULL;
    if (0 == gVm->AttachCurrentThread(&env, NULL)) {
        NativeWorkerArgs* nativeWorkerAgrs = (NativeWorkerArgs*) args;

        //
        nativeWorker(env, gObj, nativeWorkerAgrs->id,
                     nativeWorkerAgrs->iterations);

        delete nativeWorkerAgrs;

        gVm->DetachCurrentThread();
    }

    return (void*) 1;
}

//java调用的，启动多个线程
void  FUN(posixThreads)(JNIEnv *env, jobject obj,jint threads, jint iterations) {

    //thread handlers
    pthread_t* handles = new pthread_t[threads];

    //启动线程
    for (jint i = 0; i < threads; i++) {

        //thread arguments
        NativeWorkerArgs* nativeWorkArgs = new NativeWorkerArgs();
        nativeWorkArgs->id = i;
        nativeWorkArgs->iterations = iterations;

        //thread handler
        int result = pthread_create(&handles[i], NULL, nativeWorkerThread,(void*) nativeWorkArgs);

        if (result != 0) {
            //异常
            jclass exceptionClazz = env->FindClass(
                    "java/lang/RuntimeException");
            //抛出
            env->ThrowNew(exceptionClazz, "Unable to create thread--");
            return;
        }
    }

    //线程运行结果
    for (jint i = 0; i < threads; i++) {
        void** result = NULL;
        //用来等待一个线程的结束,线程间同步的操作
        if (0 != pthread_join(handles[i], result)) {
            //异常
            jclass exceptionClazz = env->FindClass(
                    "java/lang/RuntimeException");
            //抛出
            env->ThrowNew(exceptionClazz, "Unable to join thread--");
        } else {
            char message[26];
            sprintf(message, "Worker %d:return %d", i, result);

            jstring messageString = env->NewStringUTF(message);
            env->CallVoidMethod(obj, gOnNativeMessage, messageString);

            if (NULL != env->ExceptionOccurred()) {
                return;
            }
        }
    }

}