LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES += $(LOCAL_PATH)/include  \
                    $(LOCAL_PATH)/src


LOCAL_MODULE    := AVIPlayer
LOCAL_SRC_FILES := \
	src/Common.cpp \
	src/com_example_elvin_unit12_AbstractPlayerActivity.cpp \
	src/com_example_elvin_unit12_BitmapPlayerActivity.cpp

# Add NEON optimized version on armeabi-v7a
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
	LOCAL_SRC_FILES += src/BrightnessFilter.cpp.neon
	LOCAL_STATIC_LIBRARIES += cpufeatures
else
	LOCAL_SRC_FILES += src/BrightnessFilter.cpp
endif

# Use AVILib static library 
LOCAL_STATIC_LIBRARIES += avilib_static

# Android NDK Profiler enabled
MY_ANDROID_NDK_PROFILER_ENABLED := true

# If Android NDK Profiler is enabled
ifeq ($(MY_ANDROID_NDK_PROFILER_ENABLED),true)

# Show message
$(info GNU Profiler is enabled)

# Enable the monitor functions
LOCAL_CFLAGS += -DMY_ANDROID_NDK_PROFILER_ENABLED

# Use Android NDK Profiler static library
LOCAL_CFLAGS += -pg
LOCAL_STATIC_LIBRARIES += android-ndk-profiler
endif

# Link with JNI graphics
LOCAL_LDLIBS += -ljnigraphics

include $(BUILD_SHARED_LIBRARY)

# Import AVILib library module
$(call import-module,transcode-1.1.5/avilib)

# If Android NDK Profiler is enabled
ifdef MY_ANDROID_NDK_PROFILER_ENABLED
# Import Android NDK Profiler library module
$(call import-module, android-ndk-profiler/jni)
endif

# Add CPU features on armeabi-v7a
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
# Import Android CPU features
$(call import-module, android/cpufeatures)
endif
