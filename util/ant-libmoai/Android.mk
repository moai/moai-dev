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

	MY_LOCAL_CFLAGS		:=
	MY_INCLUDES			:=
	
	@GLOBALS@

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

	@MODULES@

#================================================================#
# targets
#================================================================#

	include libraries.mk
	include $(MY_INCLUDES)
