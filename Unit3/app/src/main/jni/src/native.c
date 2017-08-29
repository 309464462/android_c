//
// Created by elvin on 2017/8/28.
//

#include "com_example_elvin_unit3_jni_JniTest.h"

#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}

JNIEXPORT jstring JNICALL Java_com_example_elvin_unit3_jni_JniTest_strFromJni
          (JNIEnv *env ,jobject thiz, jstring javastring){
//            return (*env)->NewString(env,"asdf",4);  //这是unicode
    jclass  clazz = (*env)->GetObjectClass(env,thiz);
    jmethodID instanceMethodId;
    instanceMethodId = (*env)->GetMethodID(env,clazz,"instanceMethod","()Ljava/lang/String;");
    jstring instanceResult;
    instanceResult = (*env)->CallObjectMethod(env,thiz,instanceMethodId);

    jmethodID staticMethodId;
    staticMethodId = (*env)->GetStaticMethodID(env,clazz,"staticMethod","()Ljava/lang/String;");
    jstring staticResult;
    staticResult = (*env)->CallStaticObjectMethod(env,clazz,staticMethodId);  //这里第二个参数调用的是静态域


//        return (*env)->NewStringUTF(env,"asdf");  //这是utf
    return staticResult;
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

JNIEXPORT jstring JNICALL Java_com_example_elvin_unit3_jni_JniTest_getFiledFromJava
        (JNIEnv *env, jobject thiz){
    jclass clazz;
    clazz = (*env)->GetObjectClass(env,thiz);
    //获取id
    jfieldID instanceFieldId;
    instanceFieldId = (*env)->GetFieldID(env,clazz,"instanceFiled","Ljava/lang/String;");
    jfieldID staticFieldId;
    staticFieldId = (*env)->GetStaticFieldID(env,clazz,"staticFiled","Ljava/lang/String;");
    //获取实例域
    jstring instanceField;
    instanceField = (*env)->GetObjectField(env,thiz,instanceFieldId);
    jstring staticField;
    staticField = (*env)->GetStaticObjectField(env,clazz,staticFieldId);

     return instanceField;
}

JNIEXPORT void JNICALL Java_com_example_elvin_unit3_jni_JniTest_testThrowExecption
        (JNIEnv *env, jobject thiz){
    jthrowable ex;
    jclass clazz;
    clazz = (*env)->GetObjectClass(env,thiz);
    jmethodID exceptionMethodId;
    exceptionMethodId = (*env)->GetStaticMethodID(env,clazz,"testThrowExecption","()V;");
    (*env)->CallVoidMethod(env,thiz,exceptionMethodId);
    ex = (*env)->ExceptionOccurred(env);
    if( 0 != ex ){
        (*env)->ExceptionClear(env);
    }
}


void MultiThread(JNIEnv *env, jobject thiz){
    if(JNI_OK == (*env)->MonitorEnter(env,thiz)){
        //错误处理
    }

    //同步代码块
    if(JNI_OK == (*env)->MonitorExit(env,thiz)){
        // 错误处理
    }
}


void jniThread(JNIEnv *env, jobject thiz){
    JavaVM * cacheJVM;

    //将线程附着到虚拟机
    (*cacheJVM)->AttachCurrentThread(cacheJVM,&env,NULL);
    // 可以用JNIENV 接口实现线程与java应用程序之间的通信
    //将当前线程与虚拟机分离
    (*cacheJVM)->DetachCurrentThread(cacheJVM);
}

