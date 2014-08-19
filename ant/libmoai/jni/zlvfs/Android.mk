#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= zlvfs
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_CFLAGS		:= -fvisibility=hidden
	
	LOCAL_SRC_FILES 	+= $(wildcard $(MY_MOAI_ROOT)/src/zl-vfs/*.cpp)
	
	include $(BUILD_STATIC_LIBRARY)