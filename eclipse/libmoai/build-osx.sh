#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#
#!/bin/bash

echo ----------------------------------------------------------------
echo Building libmoai...
echo ----------------------------------------------------------------

cd libcrypto-osx
./build.sh

cd ../
mv libcrypto-osx/obj/local/armeabi/libcrypto.a jni

cd jni
ndk-build