#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= ssl
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
	LOCAL_CFLAGS 		:= -DOPENSSL_NO_DEPRECATED
	LOCAL_CFLAGS		+= -include $(MY_MOAI_ROOT)/src/zlcore/zl_replace.h

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)

	include $(BUILD_STATIC_LIBRARY)