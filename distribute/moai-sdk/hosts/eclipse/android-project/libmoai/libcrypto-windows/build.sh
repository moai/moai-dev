#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#
#!/bin/bash

cd jni
ndk-build

cd ../obj/local/armeabi

ar -x libcrypto-a.a
ar -x libcrypto-b.a
ar -x libcrypto-c.a
ar rcs libcrypto.a *.o
ranlib libcrypto.a