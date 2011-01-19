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
LOCAL_MODULE := sdl
LOCAL_CFLAGS	:= -g
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_EXPORT_CFLAGS := -D__LINUX__
#LOCAL_EXPORT_LDLIBS := -lsdl

LOCAL_SRC_FILES := src/audio/SDL_wave.c \
		src/audio/SDL_audio.c \
		src/audio/SDL_mixer.c \
		src/audio/SDL_mixer_m68k.c \
		src/audio/dummy/SDL_dummyaudio.c \
		src/audio/SDL_audiocvt.c \
		src/audio/SDL_mixer_MMX.c \
		src/audio/SDL_mixer_MMX_VC.c \
		src/audio/SDL_audiodev.c \
		src/SDL.c \
		src/thread/generic/SDL_systhread.c \
		src/thread/generic/SDL_syscond.c \
		src/thread/generic/SDL_sysmutex.c \
		src/thread/generic/SDL_syssem.c \
		src/thread/SDL_thread.c \
		src/events/SDL_quit.c \
		src/events/SDL_expose.c \
		src/events/SDL_mouse.c \
		src/events/SDL_events.c \
		src/events/SDL_active.c \
		src/events/SDL_keyboard.c \
		src/events/SDL_resize.c \
		src/video/SDL_blit.c \
		src/video/SDL_blit_1.c \
		src/video/SDL_yuv_mmx.c \
		src/video/SDL_blit_N.c \
		src/video/SDL_pixels.c \
		src/video/SDL_blit_A.c \
		src/video/dummy/SDL_nullmouse.c \
		src/video/dummy/SDL_nullevents.c \
		src/video/dummy/SDL_nullvideo.c \
		src/video/SDL_yuv.c \
		src/video/SDL_stretch.c \
		src/video/SDL_yuv_sw.c \
		src/video/SDL_blit_0.c \
		src/video/SDL_surface.c \
		src/video/SDL_RLEaccel.c \
		src/video/SDL_video.c \
		src/video/SDL_bmp.c \
		src/video/SDL_cursor.c \
		src/video/SDL_gamma.c \
		src/SDL_fatal.c \
		src/cdrom/dummy/SDL_syscdrom.c \
		src/cdrom/SDL_cdrom.c \
		src/loadso/dlopen/SDL_sysloadso.c \
		src/file/SDL_rwops.c \
		src/timer/unix/SDL_systimer.c \
		src/timer/SDL_timer.c \
		src/joystick/dummy/SDL_sysjoystick.c \
		src/joystick/SDL_joystick.c \
		src/SDL_error.c \
		src/cpuinfo/SDL_cpuinfo.c \
		src/stdlib/SDL_getenv.c src/stdlib/SDL_iconv.c src/stdlib/SDL_malloc.c src/stdlib/SDL_qsort.c src/stdlib/SDL_stdlib.c src/stdlib/SDL_string.c
	
include $(BUILD_STATIC_LIBRARY)

