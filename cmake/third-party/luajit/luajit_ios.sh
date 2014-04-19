#!/bin/sh
#--------------------------------------------------------------------------------------
# Copyright (c) 2010-2013 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#--------------------------------------------------------------------------------------

MOAI_ROOT="$(dirname $0)/../../../"


LIPO="xcrun -sdk iphoneos lipo"
STRIP="xcrun -sdk iphoneos strip"

# Usage
if [ "$1" = "help" ]; then
    echo "Usage:\n $0 [product_name] [src_dir] [output_directory] [vfs_flags]"
    exit 0
fi

# Process Input Arguments
if [ "x$1" != "x" ]; then
    PRODUCT_NAME="$1"
else
    PRODUCT_NAME=libmoai-ios-luajit.a
fi
if [ "x$2" != "x" ]; then
    SRCDIR="$2"
else
    SRCDIR=$MOAI_ROOT/3rdparty/LuaJIT-2.0.3
fi

if [ "x$3" != "x" ]; then
    DESTDIR="$3"
else
    DESTDIR=$MOAI_ROOT/xcode/libmoai/luaJIT/ios
fi

if [ "x$4" != "x" ]; then
    CFLAGS="$4"
else
    CFLAGS="-I$MOAI_ROOT/src -include $MOAI_ROOT/src/zl-vfs/zl_replace.h"
fi

if [ -f "$DESTDIR"/$PRODUCT_NAME ]; then
    echo "LuaJIT already exists, exiting."
    exit 0
fi


IXCODE=`xcode-select -print-path`
ISDK=$IXCODE/Platforms/iPhoneOS.platform/Developer
ISDKVER=iPhoneOS${IPHONEOS_DEPLOYMENT_TARGET}.sdk
ISDKP=$ISDK/usr/bin/

SIMDIR=$IXCODE/Platforms/iPhoneSimulator.platform/Developer
SIMBIN=$SIMDIR/usr/bin/
SIMVER=iPhoneSimulator${IPHONEOS_DEPLOYMENT_TARGET}.sdk

mkdir -p $DESTDIR
rm "$DESTDIR"/*.a 2>/dev/null
cd $SRCDIR

CC_ARGS="IPHONEOS_DEPLOYMENT_TARGET='' xcrun -sdk macosx clang -m32 -arch i386"

LUAJIT_LIB=libluajit.a

crossCompile()
{
    ARCH=$1
    ISDKF="-arch $ARCH -isysroot $ISDK/SDKs/$ISDKVER"
    make BUILDTYPE=static CC="clang" HOST_CC="$CC_ARGS" CROSS="xcrun --sdk iphoneos " TARGET_CONLY_FLAGS="$CFLAGS" TARGET_FLAGS="$ISDKF" TARGET_SYS=iOS clean
    make -j BUILDTYPE=static CC="clang" HOST_CC="$CC_ARGS" CROSS="xcrun --sdk iphoneos " TARGET_CONLY_FLAGS="$CFLAGS" TARGET_FLAGS="$ISDKF" TARGET_SYS=iOS
    if [ $? -eq 0 ]; then
        mv "$SRCDIR"/src/$LUAJIT_LIB "$DESTDIR"/libluajit-$ARCH.a
    else
        echo "Error building LuaJIT for $ARCH.  Exiting."
        exit 1
    fi
}

compile() 
{   
    
    #  make HOST_CC="$CC_ARGS" clean
    make -j BUILDTYPE=static CC="clang" HOST_CC="$CC_ARGS" HOST_CFLAGS="-arch i386" HOST_LDFLAGS="-arch i386" STATIC_CC="clang" TARGET_SYS=iOS TARGET=x86 CROSS="xcrun --sdk iphonesimulator " TARGET_FLAGS="-isysroot $SIMDIR/SDKs/$SIMVER -arch i386"  TARGET_CONLY_FLAGS="$CFLAGS" clean
    make -j BUILDTYPE=static CC="clang" HOST_CC="$CC_ARGS" HOST_CFLAGS="-arch i386" HOST_LDFLAGS="-arch i386" STATIC_CC="clang" TARGET_SYS=iOS TARGET=x86 CROSS="xcrun --sdk iphonesimulator " TARGET_FLAGS="-isysroot $SIMDIR/SDKs/$SIMVER -arch i386"  TARGET_CONLY_FLAGS="$CFLAGS"
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

