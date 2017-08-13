# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)
#
# 第三方库
#

include $(CLEAR_VARS)

LOCAL_C_INCLUDES += $(LOCAL_PATH)/include  \
                    $(LOCAL_PATH)/src

LOCAL_SRC_FILES := src/staticlib.c

LOCAL_MODULE    := StaticLib

include $(BUILD_SHARED_LIBRARY)

#
# 目标库
#

include $(CLEAR_VARS)

LOCAL_C_INCLUDES += $(LOCAL_PATH)/include  \
                    $(LOCAL_PATH)/src

LOCAL_SRC_FILES := src/hello-jni.c

LOCAL_MODULE    := NativeLib

LOCAL_SHARED_LIBRARIES := StaticLib

include $(BUILD_SHARED_LIBRARY)


#
# 目标库
#

include $(CLEAR_VARS)

LOCAL_C_INCLUDES += $(LOCAL_PATH)/include  \
                    $(LOCAL_PATH)/src

LOCAL_SRC_FILES := src/hello-jni2.c

LOCAL_MODULE    := NativeLib2

LOCAL_SHARED_LIBRARIES := StaticLib

include $(BUILD_SHARED_LIBRARY)