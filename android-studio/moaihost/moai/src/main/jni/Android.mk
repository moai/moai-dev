#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	LOCAL_PATH := $(call my-dir)
	
	include $(CLEAR_VARS)
	
	MOAI_SDK_HOME	:= ../../../../../../
	MY_ARM_MODE		:= arm
	MY_ARM_ARCH		:= armeabi-v7a x86

	MY_LOCAL_CFLAGS		:=
	MY_INCLUDES			:=
	
	MOAI_MODULES	:= ../../../../../../util/ant-libmoai/

	#----------------------------------------------------------------#
	# recursive wildcard function
	#----------------------------------------------------------------#

	rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)$(filter $(subst *,%,$2),$d))

#================================================================#
# 3rd party (core)
#================================================================#

	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/contrib
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/expat-2.1.0/amiga
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/expat-2.1.0/lib
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/expat-2.1.0/xmlwf
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/jansson-2.1/src
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/lua-5.1.3/src
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/ooid-0.99
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/sfmt-1.4
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/sqlite-3.6.16
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/tinyxml
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/tlsf-2.0
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/zlib-1.2.3

	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-contrib.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-expat.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-json.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-lua.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-sfmt.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-sqlite.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-tinyxml.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-zlib.mk

#================================================================#
# moai core
#================================================================#

	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/src/zl-vfs
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/src
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/src/config-default

	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/zl-core.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/zl-vfs.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/moai-core.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/moai-util.mk

#================================================================#
# moai modules
#================================================================#

	#--------------------------------------------------------------#
	# ANDROID

	MY_HEADER_SEARCH_PATHS += 
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/moai-android.mk

	#--------------------------------------------------------------#
	# BOX2D

	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/box2d-2.3.0/
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/box2d-2.3.0/Box2D
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/box2d-2.3.0/Box2D/Collision/Shapes
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/box2d-2.3.0/Box2D/Collision
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/box2d-2.3.0/Box2D/Common
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/box2d-2.3.0/Box2D/Dynamics
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/box2d-2.3.0/Box2D/Dynamics/Contacts
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/box2d-2.3.0/Box2D/Dynamics/Joints
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-box2d.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/moai-box2d.mk

	#--------------------------------------------------------------#
	# CRYPTO

	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/include-android
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/include
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-crypto-a.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-crypto-b.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-crypto-c.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-crypto-d.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/zl-crypto.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/moai-crypto.mk

	#--------------------------------------------------------------#
	# HTTP_CLIENT

	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/include-android
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/curl-7.19.7/include-android
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/include-android
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/include
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-c-ares.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-curl.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-ssl.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/moai-http-client.mk

	#--------------------------------------------------------------#
	# LUAEXT

	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/luacrypto-0.2.0/src
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/luacurl-1.2.1
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/luafilesystem-1.5.0/src
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/luasocket-2.0.2/src
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/luasocket-2.0.2-embed/src
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/luasql-2.2.0/src
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/moai-luaext.mk

	#--------------------------------------------------------------#
	# SIM

	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/freetype-2.4.4/include
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/freetype-2.4.4/include/freetype
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/freetype-2.4.4/include/freetype2
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/freetype-2.4.4/builds
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/freetype-2.4.4/src
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/freetype-2.4.4/config
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/libtess2/Include
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/jpeg-8c
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/lpng140
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/libpvr-3.4
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-freetype.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-jpg.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-png.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-pvr.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/3rdparty-tess.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/zl-gfx.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/util/ant-libmoai/modules/moai-sim.mk


#================================================================#
# targets
#================================================================#

	include libraries.mk
	include $(MY_INCLUDES)
