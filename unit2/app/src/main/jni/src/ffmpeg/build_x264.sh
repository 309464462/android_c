#!/bin/bash 
NDK=D:/Androidsdk/ndk-bundle
SYSROOT=$NDK/platforms/android-19/arch-arm/
TOOLCHAIN=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/windows-x86_64
EXTRA_LIB=$(pwd)/android-lib
function build_one {
./configure \
     --prefix=$PREFIX \
     --enable-asm \
     --enable-neon \
     --enable-static \
     --enable-small \
     --enable-libx264 \
     --enable-gpl \
     --enable-encoder=libx264 \
     --disable-shared \
     --disable-doc \
     --disable-ffmpeg \
     --disable-ffplay \
     --disable-ffprobe \
     --disable-ffserver \
     --disable-avdevice \
     --disable-symver \
     --cross-prefix=$TOOLCHAIN/bin/arm-linux-androideabi- \
     --target-os=linux \
     --arch=arm \
     --cpu=armv7-a \
     --enable-cross-compile \
     --sysroot=$SYSROOT \
     --extra-cflags="-I$EXTRA_LIB/include -fPIC -DANDROID -mfpu=neon -mfloat-abi=softfp -I$NDK/platforms/android-19/arch-arm/usr/include" \
     --extra-ldflags="-L$EXTRA_LIB/lib -lx264 $ADDI_LDFLAGS"

     make clean
     make j4
     make install
 }
 CPU=arm
 PREFIX=$(pwd)/android/x264_lib/$CPU
 ADDI_CFLAGS="-marm"
 build_one