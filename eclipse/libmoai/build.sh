#!/bin/bash

#----------------------------------------------------------------
# build
#----------------------------------------------------------------
cd ../libcrypto
./build.sh

cd ../
mv libcrypto/bin/libcrypto.a libmoai/jni

cd libmoai/jni
ndk-build