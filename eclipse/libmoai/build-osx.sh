#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#
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