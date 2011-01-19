# Copyright (C) 2010 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.c \rg/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_ARM_MODE := arm
TARGET_ARCH_ABI := armeabi-v7a
LOCAL_MODULE := dummy_audio
LOCAL_CFLAGS	:= -D__LINUX__ -DNEW_DYNAREC -DDYNAREC -D__arm__ -g -O3 -D__ANDROID__ -DNO_GUI=1 -pipe -ffast-math -funroll-loops -fexpensive-optimizations -fno-strict-aliasing -mfpu=vfp -mfloat-abi=softfp
LOCAL_C_INCLUDES := $(LOCAL_PATH)../
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)../
LOCAL_EXPORT_CFLAGS := -D__LINUX__

LOCAL_SRC_FILES := audio.c
		
include $(BUILD_SHARED_LIBRARY)

