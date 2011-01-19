#!/bin/sh
/home/cpasjuste/dev/android/android-ndk-r5/toolchains/arm-linux-androideabi-4.4.3/prebuilt/linux-x86/bin/arm-linux-androideabi-gdb ./libs/armeabi-v7a/mupen64plus 
set solib-search-path /home/cpasjuste/dev/android/droid64/obj/local/armeabi-v7a
directory /home/cpasjuste/dev/android/android-ndk-r5/platforms/android-9/arch-arm/usr/include /home/cpasjuste/dev/android/android-ndk-r5/sources/android/native_app_glue /home/cpasjuste/dev/android/droid64/jni/SDL/include /home/cpasjuste/dev/android/droid64/jni/libpng /home/cpasjuste/dev/android/droid64/jni//SDL/include /home/cpasjuste/dev/android/android-ndk-r5/sources/cxx-stl/system/include /home/cpasjuste/dev/android/droid64/jni /home/cpasjuste/dev/android/droid64/jni//SDL /home/cpasjuste/dev/android/android-ndk-r5/sources/cxx-stl/system
