LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)


LOCAL_C_INCLUDES += $(LOCAL_PATH)/include  \
                    $(LOCAL_PATH)/src

LOCAL_SRC_FILES := src/native.c


LOCAL_MODULE    := native

MY_LOG_TAG := \"logtest\"

# 定义基于构建类型的默认日志等级
ifeq ($(APP_OPTIIM),release)
  MY_LOG_LEVEL := MY_LOG_LEVEL_ERROR
else
  MY_LOG_LEVEL := MY_LOG_LEVEL_VERBOSE
endif

# 追加编译标志
#LOCAL_CFLAGS += -DMY_LOG_TAG=$(MY_LOG_TAG)
#LOCAL_CFLAGS += -DMY_LOG_LEVEL=$(MY_LOG_LEVEL)

LOCAL_PROGUARD_ENABLED:= disabled

LOCAL_LDLIBS := -llog


include $(BUILD_SHARED_LIBRARY)