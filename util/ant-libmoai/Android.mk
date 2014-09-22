#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	LOCAL_PATH := $(call my-dir)
	
	include $(CLEAR_VARS)
	
	MOAI_SDK_HOME	:= @MOAI_SDK_HOME@
	MY_ARM_MODE		:= @MY_ARM_MODE@
	MY_ARM_ARCH		:= @MY_ARM_ARCH@
	@GLOBALS@

	#----------------------------------------------------------------#
	# recursive wildcard function
	#----------------------------------------------------------------#

	rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)$(filter $(subst *,%,$2),$d))

	LOCAL_MODULE 		:= moai
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
	LOCAL_LDLIBS 		:= -llog -lGLESv1_CM -lGLESv2
	LOCAL_CFLAGS		:=
	MY_LOCAL_CFLAGS		:=
	MY_INCLUDES			:=

#================================================================#
# core
#================================================================#

	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/src/zl-vfs
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/src
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/src/config-default

	#----------------------------------------------------------------#
	# ZL

	MY_INCLUDES += $(MOAI_SDK_HOME)/ant/libmoai/modules/zl-core.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/ant/libmoai/modules/zl-gfx.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/ant/libmoai/modules/zl-vfs.mk

	#----------------------------------------------------------------#
	# ANDROID-APP

	MY_INCLUDES += $(MOAI_SDK_HOME)/ant/libmoai/modules/moai-android.mk
	
	#----------------------------------------------------------------#
	# MOAI
	
	MY_INCLUDES += $(MOAI_SDK_HOME)/ant/libmoai/modules/moai-core.mk

#================================================================#
# 3rd party (core)
#================================================================#

	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/contrib
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/expat-2.0.1/amiga
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/expat-2.0.1/lib
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/expat-2.0.1/xmlwf
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/jansson-2.1/src
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/lua-5.1.3/src
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/ooid-0.99
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/sfmt-1.4
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/sqlite-3.6.16
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/tinyxml
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/tlsf-2.0
	MY_HEADER_SEARCH_PATHS += $(MOAI_SDK_HOME)/3rdparty/zlib-1.2.3

	MY_INCLUDES += $(MOAI_SDK_HOME)/ant/libmoai/modules/3rdparty-contrib.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/ant/libmoai/modules/3rdparty-expat.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/ant/libmoai/modules/3rdparty-json.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/ant/libmoai/modules/3rdparty-lua.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/ant/libmoai/modules/3rdparty-sfmt.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/ant/libmoai/modules/3rdparty-sqlite.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/ant/libmoai/modules/3rdparty-tinyxml.mk
	MY_INCLUDES += $(MOAI_SDK_HOME)/ant/libmoai/modules/3rdparty-zlib.mk

#================================================================#
# modules
#================================================================#

	@MODULES@

#================================================================#
# source files
#================================================================#

	LOCAL_CFLAGS		:= $(MY_LOCAL_CFLAGS) -DAKU_WITH_PLUGINS=1 -include $(MOAI_SDK_HOME)/src/zl-vfs/zl_replace.h
	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	
	LOCAL_SRC_FILES 	+= src/jni.cpp
	LOCAL_SRC_FILES 	+= $(wildcard $(MOAI_SDK_HOME)src/host-modules/*.cpp)
	LOCAL_SRC_FILES 	+= src/aku_plugins.cpp

	LOCAL_STATIC_LIBRARIES := @STATIC_LIBRARIES@
	LOCAL_WHOLE_STATIC_LIBRARIES := @WHOLE_STATIC_LIBRARIES@

#----------------------------------------------------------------#
# build shared library
#----------------------------------------------------------------#

	include $(BUILD_SHARED_LIBRARY)

#----------------------------------------------------------------#
# include submodules
#----------------------------------------------------------------#
	
	include $(MY_INCLUDES)
