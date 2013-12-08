#!/bin/sh
#--------------------------------------------------------------------------------------
# Copyright (c) 2010-2013 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#--------------------------------------------------------------------------------------

MOAI_ROOT="$( cd ../.. && pwd )"
SRCDIR=$MOAI_ROOT/3rdparty/LuaJIT-2.0.1
DESTDIR=$MOAI_ROOT/xcode/libmoai/luaJIT/osx
MACOSX_DEPLOYMENT_TARGET="10.6"

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

mkdir -p $DESTDIR
rm "$DESTDIR"/*.a 2>/dev/null

cd $SRCDIR
make clean
make CC="gcc -m32 -arch i386" clean all

if [ -f $SRCDIR/src/libluajit.a ]; then
    mv $SRCDIR/src/libluajit.a $DESTDIR/$PRODUCT_NAME
fi

make clean
