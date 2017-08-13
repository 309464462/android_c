#!/bin/bash 
NDK=D:/Androidsdk/ndk-bundle
SYSROOT=$NDK/platforms/android-19/arch-arm/
TOOLCHAIN=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/windows-x86_64
#EXTRA_LIB 指定刚刚编译的libx264的目录
#--enable-libx264 --enable-gpl 就是把libx264启用，两个都需要加
#--extra-cflags --extra-ldflags需要指定编译libx264所生成的头文件的lib文件
EXTRA_LIB=$(pwd)/android-lib
function build_one { 
./configure \
     --prefix=$PREFIX \
     --enable-asm \
     --enable-neon \
     --enable-static \
     --enable-small \
     --disable-shared \
     --disable-doc \
     --disable-ffmpeg \
     --disable-ffplay \
     --disable-ffprobe \
     --disable-ffserver \
     --disable-avdevice \
     --disable-symver \
     --disable-muxers \
     --enable-muxer=mov \
     --enable-muxer=mp4 \
     --enable-muxer=avi \
     --disable-decoders \
     --enable-decoder=aac \
     --enable-decoder=h264 \
     --enable-decoder=mpeg4 \
     --disable-demuxers \
     --enable-demuxer=h264 \
     --enable-demuxer=avi \
     --disable-parsers \
     --enable-parser=aac \
     --enable-parser=h264 \
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
 PREFIX=$(pwd)/android_min/$CPU
 ADDI_CFLAGS="-marm" 
 build_one