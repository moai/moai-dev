#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= crypto-a
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
	LOCAL_CFLAGS 		:= $(MY_CFLAGS)

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	include ../../lists/crypto-a.mk

	include $(BUILD_STATIC_LIBRARY)
