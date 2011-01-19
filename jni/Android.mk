
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
LOCAL_MODULE    := mupen64plus
LOCAL_CFLAGS	:= -D__LINUX__ -DNEW_DYNAREC -DDYNAREC -D__arm__ -g -O3 -D__ANDROID__ -DNO_GUI=1 -pipe -ffast-math -funroll-loops -fexpensive-optimizations -fno-strict-aliasing -mfpu=vfp -mfloat-abi=softfp
LOCAL_LDFLAGS 	:= --arm_linux --gnu -Xlinker --section-start -Xlinker .init=0x08000000
LOCAL_LDLIBS    := -lz -lm -ldl
#-llog -landroid -lEGL -lGLESv1_CM -lz -lm -ldl
LOCAL_C_INCLUDES := $(LOCAL_PATH)/main $(LOCAL_PATH)/SDL/include $(LOCAL_PATH)/libpng
LOCAL_STATIC_LIBRARIES := sdl android_video android_rsp
#dummy_audio dummy_video dummy_input rsp_hle android_native_app_glue 

LOCAL_SRC_FILES := main/main.c \
	main/romcache.c \
	main/util.c \
	main/cheat.c \
	main/config.c \
	main/adler32.c \
	main/md5.c \
	main/rom.c \
	main/savestates.c \
	main/zip/ioapi.c \
	main/zip/unzip.c \
	main/bzip2/bzlib.c \
	main/bzip2/crctable.c \
	main/bzip2/decompress.c \
	main/bzip2/compress.c \
	main/bzip2/randtable.c \
	main/bzip2/huffman.c \
	main/bzip2/blocksort.c \
	main/lzma/buffer.c \
	main/lzma/io.c \
	main/lzma/main.c \
	main/7zip/7zAlloc.c \
	main/7zip/7zBuffer.c \
	main/7zip/7zCrc.c \
	main/7zip/7zDecode.c \
	main/7zip/7zExtract.c \
	main/7zip/7zHeader.c \
	main/7zip/7zIn.c \
	main/7zip/7zItem.c \
	main/7zip/7zMain.c \
	main/7zip/LzmaDecode.c \
	main/7zip/BranchX86.c \
	main/7zip/BranchX86_2.c \
	memory/dma.c \
	memory/flashram.c \
	memory/memory.c \
	memory/pif.c \
	memory/tlb.c \
	r4300/r4300.c \
	r4300/bc.c \
	r4300/compare_core.c \
	r4300/cop0.c \
	r4300/cop1.c \
	r4300/cop1_d.c \
	r4300/cop1_l.c \
	r4300/cop1_s.c \
	r4300/cop1_w.c \
	r4300/exception.c \
	r4300/interupt.c \
	r4300/profile.c \
	r4300/pure_interp.c \
	r4300/recomp.c \
	r4300/special.c \
	r4300/regimm.c \
	r4300/tlb.c \
	r4300/empty_dynarec.c \
      	r4300/new_dynarec/new_dynarec.c \
      	r4300/new_dynarec/fpu.c \
      	r4300/new_dynarec/linkage_arm.S \
	opengl/osd.cpp \
	opengl/screenshot.cpp \
	main/translate.c \
	android/plugin.c
	

#include $(BUILD_SHARED_LIBRARY)
include $(BUILD_EXECUTABLE)

#$(call import-module,android/native_app_glue)
$(call import-module,SDL)
$(call import-module,dummy_audio)
$(call import-module,android_video)
$(call import-module,dummy_input)
$(call import-module,android_rsp)

