#!/bin/bash

#----------------------------------------------------------------
# build
#----------------------------------------------------------------
cd libcrypto-osx
./build.sh

cd ../
mv libcrypto-osx/obj/local/armeabi/libcrypto.a jni

cd jni
ndk-build