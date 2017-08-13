#!/bin/bash
NDK=D:/Androidsdk/ndk-bundle
SYSROOT=$NDK/platforms/android-19/arch-arm/
TOOLCHAIN=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/windows-x86_64

function build_one { 
./configure \
     --prefix=$PREFIX \
     --enable-asm \
     --enable-neon \
     --enable-static \
     --enable-small \
     --disable-shared \
     --disable-ffmpeg \
     --disable-ffplay \
     --disable-ffprobe \
     --disable-ffserver \
     --disable-avdevice \
     --disable-doc \
     --disable-symver \
     --cross-prefix=$TOOLCHAIN/bin/arm-linux-androideabi- \
     --target-os=linux \
     --arch=arm \
     --cpu=armv7-a \
     --enable-cross-compile \
     --sysroot=$SYSROOT \
     --extra-cflags="-fPIC -DANDROID -mfpu=neon -mfloat-abi=softfp -I$NDK/platforms/android-19/arch-arm/usr/include" \
     --extra-ldflags="$ADDI_LDFLAGS" 
     
     make clean 
     make 
     make install
 }
 CPU=arm
 PREFIX=$(pwd)/android/$CPU
 ADDI_CFLAGS="-marm" 
 build_one