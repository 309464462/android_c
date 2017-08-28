LOCAL_PATH := $(call my-dir)



include $(CLEAR_VARS)



LOCAL_C_INCLUDES += $(LOCAL_PATH)/include  \
                    $(LOCAL_PATH)/src



LOCAL_SRC_FILES := src/native.c



LOCAL_MODULE    := native



include $(BUILD_SHARED_LIBRARY)