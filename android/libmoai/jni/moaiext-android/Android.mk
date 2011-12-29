#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= moaiext-android
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
	LOCAL_CFLAGS		:= -include $(MY_MOAI_ROOT)/src/zipfs/zipfs_replace.h

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)	
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moaiext-android/MOAIApp.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moaiext-android/MOAITapjoy.cpp

	include $(BUILD_STATIC_LIBRARY)