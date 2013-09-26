#!/bin/sh
#================================================================#
# Copyright (c) 2010-2013 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

MOAI_ROOT="$( cd ../../../../ && pwd )"
host_os=`uname -s | tr "[:upper:]" "[:lower:]"`
host_arch=`uname -m`

SRCDIR=$MOAI_ROOT/3rdparty/LuaJIT-2.0.1

if [ -f $SRCDIR/android/armeabi/libluajit.a ] &&
    [ -f $SRCDIR/android/armeabi-v7a/libluajit.a ] &&
    [ -f $SRCDIR/android/x86/libluajit.a ]; then
    echo "LuaJIT Already built"
    exit 0
fi

cd "$SRCDIR"

NDK_BUILD_LOCATION="$(which ndk-build)"
if [[ x$NDK_BUILD_LOCATION = "x" ]]; then
    echo "The Android NDK must be on your path."
    exit 1
fi

NDK="${NDK_BUILD_LOCATION%/ndk-build}"

if [[ x$NDK = "x" ]]; then
    echo "The Android NDK must be on your path."
    exit 1
fi

NDKABI=8
NDKVER=$NDK/toolchains/arm-linux-androideabi-4.6
NDKP=$NDKVER/prebuilt/${host_os}-${host_arch}/bin/arm-linux-androideabi-
NDKF="--sysroot $NDK/platforms/android-$NDKABI/arch-arm"
CFLAGS="-I $MOAI_ROOT/src -include zl-vfs/zl_replace.h "

buildLuaJIT()
{
    arch="$1"
    ndkarch="$2"
    DESTDIR=$SRCDIR/android/$arch
    mkdir -p $DESTDIR 2>/dev/null
    rm "$DESTDIR"/*.a 2>/dev/null
    make clean
    make HOST_CC="gcc -m32" CROSS=$NDKP TARGET_SYS=Linux TARGET_FLAGS="$NDKF $ndkarch" TARGET_CFLAGS="$CFLAGS" libluajit.a

    if [ -f $SRCDIR/src/libluajit.a ]; then
        mv $SRCDIR/src/libluajit.a $DESTDIR/libluajit.a
    fi;
}

cd src

# Android/ARM, armeabi (ARMv5TE soft-float), Android 2.2+ (Froyo)
buildLuaJIT armeabi

# Android/ARM, armeabi-v7a (ARMv7 VFP), Android 4.0+ (ICS)
buildLuaJIT armeabi-v7a "-march=armv7-a -mfloat-abi=softfp -Wl,--fix-cortex-a8"

# Android/x86, x86 (i686 SSE3), Android 4.0+ (ICS)
NDKABI=14
NDKVER=$NDK/toolchains/x86-4.6
NDKP=$NDKVER/prebuilt/${host_os}-${host_arch}/bin/i686-linux-android-
NDKF="--sysroot $NDK/platforms/android-$NDKABI/arch-x86"

buildLuaJIT x86

make clean
