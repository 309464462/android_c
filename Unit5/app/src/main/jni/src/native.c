//
// Created by elvin on 2017/8/29.
//
#include "com_example_elvin_unit5_jni_TestJni.h"
#include "my_log.h"
#include <stdlib.h>


JNIEXPORT void JNICALL Java_com_example_elvin_unit5_jni_TestJni_startToShowLog
        (JNIEnv *env, jobject job){
    int result ;
    //与进程交互的过程
    result = system("mkdir  /data/data/com.example.elvin.unit5/tmp");
    if(result == -1 || 127 == result){
        MY_LOG_VERBOSE("mkdir failed............... ");
    }else{
        MY_LOG_VERBOSE("mkdir success ...................");
    }

//    __android_log_write(ANDROID_LOG_VERBOSE,"testlog","simple log out");
//    my_log_verbose("log level is %d",101);
//     __android_log_assert("0 != errno","testlog","there is an error");

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