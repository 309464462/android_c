#!/bin/bash 
NDK=D:/Androidsdk/ndk-bundle
SYSROOT=$NDK/platforms/android-19/arch-arm/
TOOLCHAIN=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/windows-x86_64
function build_one { 
./configure \
     --prefix=$PREFIX \
     --enable-static \
     --enable-pic \
     --disable-asm \
     --disable-cli \
     --host=arm-linux \
     --cross-prefix=$TOOLCHAIN/bin/arm-linux-androideabi- \
     --sysroot=$SYSROOT
     
     make clean 
     make 
     make install
 }
PREFIX=$(pwd)/android-lib/
build_one