//
// Created by elvin on 2017/8/29.
//
#include "com_example_elvin_unit5_jni_TestJni.h"
#include <android/log.h>
#define LOG_TAG "logtest"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGT(...) __android_log_print(ANDROID_LOG_INFO,"alert",__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)





JNIEXPORT void JNICALL Java_com_example_elvin_unit5_jni_TestJni_startToShowLog
        (JNIEnv *env, jobject job){
//    LOGI("Failed to find class");
//    __android_log_write(ANDROID_LOG_VERBOSE,"testlog","simple log out");
//    my_log_verbose("log level is %d",101);
     __android_log_assert("0 != errno","testlog","there is an error");

}

/**
 * 参数输出日志
 * */
void my_log_verbose(const char * format,...){
    va_list args;
    va_start(args,format);
    __android_log_vprint(ANDROID_LOG_VERBOSE,"testlog",format,args);
    va_end(args);
}