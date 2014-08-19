#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= zlcore
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
	LOCAL_CFLAGS		:= -include $(MY_MOAI_ROOT)/src/zl-vfs/zl_replace.h
	LOCAL_CFLAGS		+= -fvisibility=hidden

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_SRC_FILES 	+= $(wildcard $(MY_MOAI_ROOT)/src/zl-gfx/zl_gfx_opengl.cpp)
	LOCAL_SRC_FILES 	+= $(wildcard $(MY_MOAI_ROOT)/src/zl-util/*.cpp)
	LOCAL_SRC_FILES 	+= $(wildcard $(MY_MOAI_ROOT)/src/zl-crypto/*.cpp)
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/tlsf-2.0/tlsf.c

	include $(BUILD_STATIC_LIBRARY)