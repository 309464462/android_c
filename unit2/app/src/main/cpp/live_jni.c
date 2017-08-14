//
// Created by elvin on 2017/8/5.
//
#include <jni.h>
#include "libavcodec/avcodec.h"

JNIEXPORT jstring JNICALL Java_com_example_elvin_unit2_MainActivity_helloFromFFmpeg
  (JNIEnv * env, jobject thiz){
      char info[10000] = {0};
      sprintf(info, "%s\n", avcodec_configuration());

    return (*env)->NewStringUTF(env,info);
}
