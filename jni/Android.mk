# Copyright (C) 2010 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.crg/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := native-activity
LOCAL_SRC_FILES := main/main.c \
	main/romcache.c \
	main/util.c \
	main/cheat.c \
	main/config.c \
	main/adler32.c \
	main/md5.c \
	main/plugin.c \
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
	r4300/tlb.c
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv1_CM
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
