#!/bin/bash

#----------------------------------------------------------------
# build
#----------------------------------------------------------------
cd jni
ndk-build

cd ../obj/local/armeabi

ar -x libcrypto-a.a
ar -x libcrypto-b.a
ar -x libcrypto-c.a

ar rcs libcrypto.a libcrypto-a.a libcrypto-b.a libcrypto-c.a

cd ../../../
mv obj/local/armeabi/libcrypto.a bin/