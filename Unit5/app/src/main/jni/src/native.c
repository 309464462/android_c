//
// Created by elvin on 2017/8/29.
//
#include "com_example_elvin_unit5_jni_TestJni.h"
#include "my_log.h"
#include <stdlib.h>
#include<stdio.h>


JNIEXPORT void JNICALL Java_com_example_elvin_unit5_jni_TestJni_startToShowLog
        (JNIEnv *env, jobject job){
    int result ;
    //与进程交互的过程
//    result = system("mkdir  /data/data/com.example.elvin.unit5/tmp");
//    if(result == -1 || 127 == result){
//        MY_LOG_VERBOSE("mkdir failed............... ");
//    }else{
//        MY_LOG_VERBOSE("mkdir success ...................");
//    }
    //与子进程通信 即通过管道 为父进程 和子进程之间打开一个双向通道
    FILE * stream;
    stream = popen("ls","r");
    if(NULL == stream){
        MY_LOG_VERBOSE("管道没有创建成功................");
    }else{
        char buffer[1024];
        int status;

        while(NULL != fgets(buffer,1024,stream)){
            MY_LOG_VERBOSE("stream:%s",buffer);
        }

        //关闭通道
        status = pclose(stream);
        MY_LOG_VERBOSE("关闭管道：%d",status);
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