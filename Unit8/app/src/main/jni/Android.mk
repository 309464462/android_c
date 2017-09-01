LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES += $(LOCAL_PATH)/include  \
                    $(LOCAL_PATH)/src

LOCAL_SRC_FILES := src/native.cpp

LOCAL_MODULE    := Echo

MY_LOG_TAG := \"logtest\"

# 定义基于构建类型的默认日志等级
ifeq ($(APP_OPTIIM),release)
  MY_LOG_LEVEL := MY_LOG_LEVEL_ERROR
else
  MY_LOG_LEVEL := MY_LOG_LEVEL_VERBOSE
endif

LOCAL_CFLAGS := -D__STDINT_LIMITS -DINDEPENDENT_NODE -pipe -fstrict-aliasing -pthread -fexceptions -fpermissive -fno-inline  -frtti -Wdeprecated-declarations
# 追加编译标志
LOCAL_CFLAGS += -DMY_LOG_TAG=$(MY_LOG_TAG)
LOCAL_CFLAGS += -DMY_LOG_LEVEL=$(MY_LOG_LEVEL)
#LOCAL_CPP_FEATURES += rtti

LOCAL_PROGUARD_ENABLED:= disabled

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
