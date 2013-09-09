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

if [ -f "$DESTDIR"/libluajit2.a ]; then
    echo "LuaJIT already exists, exiting."
    exit 0
fi

mkdir -p $DESTDIR
rm "$DESTDIR"/*.a 2>/dev/null

cd $SRCDIR
make clean
make CC="gcc -m32 -arch i386" clean all

if [ -f $SRCDIR/src/libluajit.a ]; then
    mv $SRCDIR/src/libluajit.a $DESTDIR/libluajit2.a
fi;

make clean
