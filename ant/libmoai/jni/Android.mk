#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	ORIGINAL_LOCAL_PATH := $(call my-dir)

	include ArmModeDefined.mk
	include OptionalComponentsDefined.mk

	include $(CLEAR_VARS)

	ifeq ($(USE_FMOD),true)
		LOCAL_PATH 				:= $(FMOD_ANDROID_SDK_ROOT)
		LOCAL_MODULE            := fmodex
		LOCAL_SRC_FILES         := api/lib/$(TARGET_ARCH_ABI)/libfmodex.so
		LOCAL_EXPORT_C_INCLUDES := api/inc

		include $(PREBUILT_SHARED_LIBRARY)

		include $(CLEAR_VARS)
	endif

	LOCAL_PATH := $(ORIGINAL_LOCAL_PATH)

	#----------------------------------------------------------------#
	# set moai root
	#----------------------------------------------------------------#

	MY_MOAI_ROOT	:= ../../..

	#----------------------------------------------------------------#
	# recursive wildcard function
	#----------------------------------------------------------------#

	rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)$(filter $(subst *,%,$2),$d))

	LOCAL_MODULE 		:= moai
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
	#LOCAL_LDLIBS 		:= -llog -lGLESv1_CM -lGLESv2 crypto/libs/$(TARGET_ARCH_ABI)/libcrypto.a ../obj/local/$(TARGET_ARCH_ABI)/libcares.a
	LOCAL_LDLIBS 		:= -llog -lGLESv1_CM -lGLESv2
	LOCAL_CFLAGS		:=
	MY_LOCAL_CFLAGS		:=
	MY_INCLUDES			:=
	
	MY_LOCAL_STATIC_LIBRARIES			:=
	MY_LOCAL_WHOLE_STATIC_LIBRARIES		:=
	LOCAL_SHARED_LIBRARIES				:=

#================================================================#
# core
#================================================================#

	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/src
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/src/config-default

	MY_INCLUDES += host-android/Android.mk
	MY_INCLUDES += moai-core/Android.mk
	MY_INCLUDES += zl-core/Android.mk
	MY_INCLUDES += zl-gfx/Android.mk
	MY_INCLUDES += zl-vfs/Android.mk

	MY_LOCAL_STATIC_LIBRARIES += libzl-core
	MY_LOCAL_STATIC_LIBRARIES += libzl-gfx
	MY_LOCAL_STATIC_LIBRARIES += libzl-vfs
	MY_LOCAL_WHOLE_STATIC_LIBRARIES += libmoai-core
	MY_LOCAL_WHOLE_STATIC_LIBRARIES += libhost-android

#================================================================#
# 3rd party
#================================================================#

	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/contrib
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/expat-2.0.1/amiga
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/expat-2.0.1/lib
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/expat-2.0.1/xmlwf
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/jansson-2.1/src
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/ooid-0.99
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/sfmt-1.4
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/sqlite-3.6.16
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/tinyxml
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/tlsf-2.0
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/zlib-1.2.3

	#MY_INCLUDES += 3rdparty-contrib/Android.mk
	#MY_INCLUDES += 3rdparty-expat/Android.mk
	#MY_INCLUDES += 3rdparty-json/Android.mk
	#MY_INCLUDES += 3rdparty-lua/Android.mk
	#MY_INCLUDES += 3rdparty-sfmt/Android.mk
	#MY_INCLUDES += 3rdparty-sqlite/Android.mk
	#MY_INCLUDES += 3rdparty-tinyxml/Android.mk
	#MY_INCLUDES += 3rdparty-zlib/Android.mk

	MY_LOCAL_STATIC_LIBRARIES += libcontrib
	MY_LOCAL_STATIC_LIBRARIES += libexpat
	MY_LOCAL_STATIC_LIBRARIES += libjson
	MY_LOCAL_STATIC_LIBRARIES += liblua
	MY_LOCAL_STATIC_LIBRARIES += libsfmt
	MY_LOCAL_STATIC_LIBRARIES += libsqlite
	MY_LOCAL_STATIC_LIBRARIES += libtinyxml
	MY_LOCAL_STATIC_LIBRARIES += libzlib

#================================================================#
# modules
#================================================================#

	MY_LOCAL_CFLAGS += -DAKU_WITH_BOX2D=$(AKU_WITH_BOX2D)
	MY_LOCAL_CFLAGS += -DAKU_WITH_CHIPMUNK=$(AKU_WITH_CHIPMUNK)
	MY_LOCAL_CFLAGS += -DAKU_WITH_CRYPTO=$(AKU_WITH_CRYPTO)
	MY_LOCAL_CFLAGS += -DAKU_WITH_FMOD_DESIGNER=$(AKU_WITH_FMOD_DESIGNER)
	MY_LOCAL_CFLAGS += -DAKU_WITH_FMOD_EX=$(AKU_WITH_FMOD_EX)
	MY_LOCAL_CFLAGS += -DAKU_WITH_HARNESS=$(AKU_WITH_HARNESS)
	MY_LOCAL_CFLAGS += -DAKU_WITH_HTTP_CLIENT=$(AKU_WITH_HTTP_CLIENT)
	MY_LOCAL_CFLAGS += -DAKU_WITH_LUAEXT=$(AKU_WITH_LUAEXT)
	MY_LOCAL_CFLAGS += -DAKU_WITH_SIM=$(AKU_WITH_SIM)
	MY_LOCAL_CFLAGS += -DAKU_WITH_TEST=$(AKU_WITH_TEST)
	MY_LOCAL_CFLAGS += -DAKU_WITH_UNTZ=$(AKU_WITH_UNTZ)
	MY_LOCAL_CFLAGS += -DAKU_WITH_UTIL=$(AKU_WITH_UTIL)

	#----------------------------------------------------------------#
	ifeq ($(AKU_WITH_BOX2D),true)
		
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/box2d-2.3.0/
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/box2d-2.3.0/Box2D
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/box2d-2.3.0/Box2D/Collision/Shapes
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/box2d-2.3.0/Box2D/Collision
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/box2d-2.3.0/Box2D/Common
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/box2d-2.3.0/Box2D/Dynamics
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/box2d-2.3.0/Box2D/Dynamics/Contacts
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/box2d-2.3.0/Box2D/Dynamics/Joints
		
		MY_INCLUDES += 3rdparty-box2d/Android.mk moai-box2D/Android.mk
		MY_LOCAL_STATIC_LIBRARIES += libbox2D libmoai-box2D
	endif
	
	#----------------------------------------------------------------#
	ifeq ($(AKU_WITH_CHIPMUNK),true)
	
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/include
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/include/chipmunk
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/include/chipmunk/constraints
		
		MY_INCLUDES += 3rdparty-chipmunk/Android.mk moai-chipmunk/Android.mk
		MY_LOCAL_STATIC_LIBRARIES += libchipmunk libmoai-chipmunk
	endif

	#----------------------------------------------------------------#
	ifeq ($(AKU_WITH_CRYPTO),true)
	
		MY_INCLUDES += 3rdparty-crypto/crypto-a/Android.mk
		MY_INCLUDES += 3rdparty-crypto/crypto-b/Android.mk
		MY_INCLUDES += 3rdparty-crypto/crypto-c/Android.mk
		MY_INCLUDES += 3rdparty-crypto/crypto-d/Android.mk
		
		MY_INCLUDES += zl-crypto/Android.mk
	
		MY_LOCAL_STATIC_LIBRARIES += libcrypto-a
		MY_LOCAL_STATIC_LIBRARIES += libcrypto-b
		MY_LOCAL_STATIC_LIBRARIES += libcrypto-c
		MY_LOCAL_STATIC_LIBRARIES += libcrypto-d
		MY_LOCAL_STATIC_LIBRARIES += libzl-crypto
	endif

	#----------------------------------------------------------------#
	ifeq ($(AKU_WITH_FMOD_EX),true)
	
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/src/moai-fmod-ex
		MY_HEADER_SEARCH_PATHS += $(FMOD_ANDROID_SDK_ROOT)/api/inc
		
		MY_INCLUDES += moai-fmod-ex/Android.mk
		MY_LOCAL_STATIC_LIBRARIES += libmoai-fmod-ex
		LOCAL_SHARED_LIBRARIES += fmodex
	endif
	
	#----------------------------------------------------------------#
	ifeq ($(AKU_WITH_HTTP_CLIENT),true)
	
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/c-ares-1.7.5
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/c-ares-1.7.5/include-android
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/include-android
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/include-android
	
		MY_INCLUDES += 3rdparty-c-ares/Android.mk 3rdparty-curl/Android.mk 3rdparty-ssl/Android.mk moai-http-client/Android.mk
		MY_LOCAL_STATIC_LIBRARIES += libcares libcurl libssl libmoai-http-client
	endif
	
	#----------------------------------------------------------------#
	ifeq ($(AKU_WITH_LUAEXT),true)
	
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/luacrypto-0.2.0/src
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/luacurl-1.2.1
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/luafilesystem-1.5.0/src
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/luasocket-2.0.2/src
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/luasocket-2.0.2-embed/src
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/luasql-2.2.0/src
		
		MY_INCLUDES += moai-luaext/Android.mk
		MY_LOCAL_STATIC_LIBRARIES += libmoai-luaext
	endif
	
	#----------------------------------------------------------------#
	ifeq ($(AKU_WITH_SIM),true)
	
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/include
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/include/freetype
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/include/freetype2
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/builds
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/config
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/libtess2/Include
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/jpeg-8c
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/lpng140
	
		MY_INCLUDES += 3rdparty-freetype/Android.mk
		MY_INCLUDES += 3rdparty-jpg/Android.mk
		MY_INCLUDES += 3rdparty-png/Android.mk
		MY_INCLUDES += 3rdparty-tess/Android.mk
		MY_INCLUDES += moai-sim/Android.mk
		
		MY_LOCAL_STATIC_LIBRARIES += libfreetype libjpg libpng libtess libmoai-sim
	endif

	#----------------------------------------------------------------#
	ifeq ($(AKU_WITH_UNTZ),true)
	
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/src/moai-untz
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/untz/include
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/untz/src
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/untz/src/native/android
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/libvorbis-1.3.2/include
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/libvorbis-1.3.2/lib
		MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/libogg-1.2.2/include
		
		MY_INCLUDES += 3rdparty-ogg/Android.mk 3rdparty-vorbis/Android.mk 3rdparty-untz/Android.mk moai-untz/Android.mk
		MY_LOCAL_STATIC_LIBRARIES += libogg libvorbis libuntz libmoai-untz
	endif
	
	#----------------------------------------------------------------#
	ifeq ($(AKU_WITH_UTIL),true)
	
		MY_INCLUDES += moai-util/Android.mk
		MY_LOCAL_STATIC_LIBRARIES += libmoai-util
	endif

#================================================================#
# source files
#================================================================#

	LOCAL_CFLAGS		:= $(MY_LOCAL_CFLAGS)
	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_SRC_FILES 	+= src/jni.cpp

	LOCAL_WHOLE_STATIC_LIBRARIES += $(MY_LOCAL_WHOLE_STATIC_LIBRARIES)
	LOCAL_STATIC_LIBRARIES += $(MY_LOCAL_STATIC_LIBRARIES)

#----------------------------------------------------------------#
# build shared library
#----------------------------------------------------------------#

	include $(BUILD_SHARED_LIBRARY)

#----------------------------------------------------------------#
# include submodules
#----------------------------------------------------------------#
	
	include $(MY_INCLUDES)
