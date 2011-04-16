#!/bin/bash
cd $1/jni
ndk-build
cp -u -f  ../libs/armeabi/$1.so ../../libMoai/obj/local/armeabi/$1.so
cd ../..