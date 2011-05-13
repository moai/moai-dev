#!/bin/bash

#----------------------------------------------------------------
# build
#----------------------------------------------------------------
cd libcrypto
./build.sh

cd ../
mv libcrypto/bin/libcrypto.a jni

cd jni
ndk-build