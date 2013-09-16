#!/bin/sh
#--------------------------------------------------------------------------------------
# Copyright (c) 2010-2013 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#--------------------------------------------------------------------------------------

MOAI_ROOT="$( cd ../.. && pwd )"
SRCDIR=$MOAI_ROOT/3rdparty/LuaJIT-2.0.1

LIPO="xcrun -sdk iphoneos lipo"
STRIP="xcrun -sdk iphoneos strip"

# Usage
if [ "$1" = "help" ]; then
    echo "Usage:\n $0 [product_name] [output_directory]"
    exit 0
fi

# Process Input Arguments
if [ "x$1" != "x" ]; then
    PRODUCT_NAME="$1"
else
    PRODUCT_NAME=libmoai-ios-luajit.a
fi

if [ "x$2" != "x" ]; then
    DESTDIR="$2"
else
    DESTDIR=$MOAI_ROOT/xcode/libmoai/luaJIT/ios
fi

if [ -f "$DESTDIR"/$PRODUCT_NAME ]; then
    echo "LuaJIT already exists, exiting."
    exit 0
fi

IXCODE=`xcode-select -print-path`
ISDK=$IXCODE/Platforms/iPhoneOS.platform/Developer
ISDKVER=iPhoneOS6.1.sdk
ISDKP=$ISDK/usr/bin/

SIMDIR=$IXCODE/Platforms/iPhoneSimulator.platform/Developer
SIMBIN=$SIMDIR/usr/bin/
SIMVER=iPhoneSimulator6.1.sdk

mkdir -p $DESTDIR
rm "$DESTDIR"/*.a 2>/dev/null
cd $SRCDIR

CC_ARGS="gcc -m32 -arch i386"
CFLAGS=""
LUAJIT_LIB=libluajit.a

crossCompile()
{
    ARCH=$1
    make clean
    ISDKF="-arch $ARCH -isysroot $ISDK/SDKs/$ISDKVER"
    make -j HOST_CC="$CC_ARGS" CROSS=$ISDKP TARGET_CFLAGS="$CFLAGS" TARGET_FLAGS="$ISDKF" TARGET_SYS=iOS
    if [ $? -eq 0 ]; then
        mv "$SRCDIR"/src/$LUAJIT_LIB "$DESTDIR"/libluajit-$ARCH.a
    else
        echo "Error building LuaJIT for $ARCH.  Exiting."
        exit 1
    fi
}

compile() 
{
    make clean
    make -j HOST_CFLAGS="-arch i386" HOST_LDFLAGS="-arch i386" TARGET_SYS=iOS TARGET=x86 clean
    make -j HOST_CFLAGS="-arch i386" HOST_LDFLAGS="-arch i386" TARGET_SYS=iOS TARGET=x86 amalg \
        CROSS=$SIMBIN TARGET_FLAGS="-isysroot $SIMDIR/SDKs/$SIMVER -arch i386"
    if [ $? -eq 0 ]; then
        mv "$SRCDIR"/src/$LUAJIT_LIB "$DESTDIR"/libluajit-i386.a
    else
        echo "Error building LuaJIT for i386.  Exiting."
        exit 1
    fi
}

crossCompile "armv7"
crossCompile "armv7s"
compile

$LIPO -create "$DESTDIR"/libluajit-*.a -output "$DESTDIR"/$PRODUCT_NAME
$STRIP -S "$DESTDIR"/$PRODUCT_NAME
$LIPO -info "$DESTDIR"/$PRODUCT_NAME

rm "$DESTDIR"/libluajit-*.a

make clean
