#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	LOCAL_PATH := $(call my-dir)
	
	include $(CLEAR_VARS)
	
	MOAI_SDK_HOME	:= ../../../
	MY_ARM_MODE		:= arm
	MY_ARM_ARCH		:= armeabi-v7a x86

	MY_LOCAL_CFLAGS		:=
	MY_INCLUDES			:=
	
	MOAI_MODULES	:= ../../../util/ant-libmoai/

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
