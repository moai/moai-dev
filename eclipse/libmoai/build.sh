#!/bin/bash

#----------------------------------------------------------------
# build
#----------------------------------------------------------------
cd libcrypto
./build.sh

cd ../
mv libcrypto/obj/local/armeabi/libcrypto.a jni

cd jni
ndk-build