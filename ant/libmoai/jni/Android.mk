#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	LOCAL_PATH := $(call my-dir)
	include $(CLEAR_VARS)

	include ArmModeDefined.mk
	include OptionalComponentsDefined.mk
	
	#----------------------------------------------------------------#
	# set moai root
	#----------------------------------------------------------------#

	MY_MOAI_ROOT	:= ../../..

	#----------------------------------------------------------------#
	# recursive wildcard function
	#----------------------------------------------------------------#

	rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)$(filter $(subst *,%,$2),$d))

	LOCAL_MODULE 	:= moai
	LOCAL_ARM_MODE 	:= $(MY_ARM_MODE)
	LOCAL_LDLIBS 	:= -llog -lGLESv1_CM -lGLESv2 crypto/libs/$(TARGET_ARCH_ABI)/libcrypto.a ../obj/local/$(TARGET_ARCH_ABI)/libogg.a
	LOCAL_CFLAGS	:= $(DISABLE_ADCOLONY) $(DISABLE_BILLING) $(DISABLE_CHARTBOOST) $(DISABLE_CRITTERCISM) $(DISABLE_FACEBOOK) $(DISABLE_NOTIFICATIONS) $(DISABLE_TAPJOY)
	
#----------------------------------------------------------------#
# header search paths
#----------------------------------------------------------------#
	
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/src
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/src/aku
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/src/config-default
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/src/moaicore
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/src/moaiext-untz
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/src/uslscore
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/src/zipfs
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/box2d-2.2.1/
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/box2d-2.2.1/Box2D
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/box2d-2.2.1/Box2D/Collision/Shapes
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/box2d-2.2.1/Box2D/Collision
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/box2d-2.2.1/Box2D/Common
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/box2d-2.2.1/Box2D/Dynamics
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/box2d-2.2.1/Box2D/Dynamics/Contacts
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/box2d-2.2.1/Box2D/Dynamics/Joints
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/c-ares-1.7.5
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/c-ares-1.7.5/include-android
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/include
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/include/chipmunk
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/include/chipmunk/constraints
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/contrib
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/include-android
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/expat-2.0.1/amiga
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/expat-2.0.1/lib
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/expat-2.0.1/xmlwf
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/include
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/include/freetype
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/include/freetype2
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/builds
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/src
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/freetype-2.4.4/config
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/jansson-2.1/src
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/jpeg-8c
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/libogg-1.2.2/include
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/libvorbis-1.3.2/include
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/libvorbis-1.3.2/lib
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/lpng140
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/luacrypto-0.2.0/src
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/luacurl-1.2.1
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/luafilesystem-1.5.0/src
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/luasocket-2.0.2/src
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/luasql-2.2.0/src
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/include-android
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/ooid-0.99
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/sqlite-3.6.16
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/tinyxml
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/tlsf-2.0
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/untz/include
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/untz/src
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/untz/src/native/android
	MY_HEADER_SEARCH_PATHS += $(MY_MOAI_ROOT)/3rdparty/3rdparty/zlib-1.2.3

#----------------------------------------------------------------#
# source files
#----------------------------------------------------------------#

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_SRC_FILES 	+= src/moai.cpp

#----------------------------------------------------------------#
# libraries
#----------------------------------------------------------------#

	LOCAL_STATIC_LIBRARIES += libaku
	LOCAL_STATIC_LIBRARIES += libmoaicore
	LOCAL_STATIC_LIBRARIES += libuslscore

	LOCAL_STATIC_LIBRARIES += libmoaiext-android
	LOCAL_STATIC_LIBRARIES += libmoaiext-luaext
	LOCAL_STATIC_LIBRARIES += libmoaiext-untz

	LOCAL_STATIC_LIBRARIES += libbox2D
	LOCAL_STATIC_LIBRARIES += libchipmunk
	LOCAL_STATIC_LIBRARIES += libcontrib
	LOCAL_STATIC_LIBRARIES += libcurl
	LOCAL_STATIC_LIBRARIES += libexpat
	LOCAL_STATIC_LIBRARIES += libfreetype
	LOCAL_STATIC_LIBRARIES += libjpg
	LOCAL_STATIC_LIBRARIES += libjson
	LOCAL_STATIC_LIBRARIES += liblua
	LOCAL_STATIC_LIBRARIES += libogg
	LOCAL_STATIC_LIBRARIES += libpng
	LOCAL_STATIC_LIBRARIES += libsqlite
	LOCAL_STATIC_LIBRARIES += libssl
	LOCAL_STATIC_LIBRARIES += libtinyxml
	LOCAL_STATIC_LIBRARIES += libvorbis
	LOCAL_STATIC_LIBRARIES += libzipfs

	include $(BUILD_SHARED_LIBRARY)

#----------------------------------------------------------------#
# include submodules
#----------------------------------------------------------------#

	include box2d/Android.mk
	include c-ares/Android.mk
	include chipmunk/Android.mk
	include contrib/Android.mk
	include curl/Android.mk
	include expat/Android.mk
	include freetype/Android.mk
	include jpg/Android.mk
	include json/Android.mk
	include lua/Android.mk
	include moaiext-android/Android.mk
	include moaiext-luaext/Android.mk
	include moaiext-untz/Android.mk
	include ogg/Android.mk
	include png/Android.mk
	include sqlite/Android.mk
	include ssl/Android.mk
	include tinyxml/Android.mk
	include vorbis/Android.mk
	include zipfs/Android.mk

	include aku/Android.mk
	include moaicore/Android.mk
	include uslscore/Android.mk