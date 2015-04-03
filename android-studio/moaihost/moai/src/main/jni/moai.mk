#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	LOCAL_PATH := $(call my-dir)
	include $(CLEAR_VARS)
	
	LOCAL_MODULE 		:= moai
	LOCAL_LDLIBS 		:= -llog -lGLESv1_CM -lGLESv2
	
	
	MY_LOCAL_CFLAGS += -DAKU_WITH_ANDROID=1
	MY_LOCAL_CFLAGS += -DAKU_WITH_ANDROID_ADCOLONY=0
	MY_LOCAL_CFLAGS += -DAKU_WITH_ANDROID_CHARTBOOST=0
	MY_LOCAL_CFLAGS += -DAKU_WITH_ANDROID_CRITTERCISM=0
	MY_LOCAL_CFLAGS += -DAKU_WITH_ANDROID_FACEBOOK=0
	MY_LOCAL_CFLAGS += -DAKU_WITH_ANDROID_FLURRY=0
	MY_LOCAL_CFLAGS += -DAKU_WITH_ANDROID_GOOGLE_PLAY=0
	MY_LOCAL_CFLAGS += -DAKU_WITH_ANDROID_TAPJOY=0
	MY_LOCAL_CFLAGS += -DAKU_WITH_ANDROID_TWITTER=0
	MY_LOCAL_CFLAGS += -DAKU_WITH_ANDROID_VUNGLE=0
	MY_LOCAL_CFLAGS += -DAKU_WITH_BOX2D=1
	MY_LOCAL_CFLAGS += -DAKU_WITH_CRYPTO=1
	MY_LOCAL_CFLAGS += -DAKU_WITH_HTTP_CLIENT=1
	MY_LOCAL_CFLAGS += -DAKU_WITH_LUAEXT=1
	MY_LOCAL_CFLAGS += -DAKU_WITH_SIM=1
	MY_LOCAL_CFLAGS += -DAKU_WITH_UNTZ=0


	LOCAL_CFLAGS		:= $(MY_LOCAL_CFLAGS) -DAKU_WITH_PLUGINS=1 -include $(MOAI_SDK_HOME)/src/zl-vfs/zl_replace.h
	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	
	LOCAL_SRC_FILES 	+= src/jni.cpp
	LOCAL_SRC_FILES 	+= $(wildcard $(MOAI_SDK_HOME)src/host-modules/*.cpp)
	LOCAL_SRC_FILES 	+= src/aku_plugins.cpp

	LOCAL_STATIC_LIBRARIES := libmoai-android libmoai-box2d libmoai-http-client libmoai-luaext libmoai-sim libmoai-crypto libzl-gfx libzl-crypto libbox2d libpvr libfreetype libjpg libpng libtess libcurl libcares libssl libcrypto-a libcrypto-b libcrypto-c libcrypto-d libmoai-util libmoai-core libzl-core libcontrib libexpat libjson liblua libsfmt libsqlite libtinyxml libzl-vfs libzlib
	LOCAL_WHOLE_STATIC_LIBRARIES := libmoai-android libmoai-sim libcrypto-a libcrypto-b libcrypto-c libcrypto-d libmoai-core

	include $(BUILD_SHARED_LIBRARY)
