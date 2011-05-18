#!/bin/bash

#----------------------------------------------------------------
# build
#----------------------------------------------------------------
cd jni
ndk-build

cd ../obj/local/armeabi

echo 1
ar -x libcrypto-a.a
echo 2
ar -x libcrypto-b.a
echo 3
ar -x libcrypto-c.a
echo 4
ar rcs libcrypto.a "*.o"
ranlib libcrypto.a
