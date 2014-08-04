#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	include OptionalComponentsDefined.mk

	LOCAL_MODULE 		:= moaiext-android
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)

	LOCAL_CFLAGS		:= $(DISABLE_ADCOLONY) $(DISABLE_BILLING) $(DISABLE_CHARTBOOST) $(DISABLE_CRITTERCISM) $(DISABLE_FACEBOOK) $(DISABLE_NOTIFICATIONS) $(DISABLE_TAPJOY) $(DISABLE_TWITTER)
	LOCAL_CFLAGS		+= -include $(MY_MOAI_ROOT)/src/zl-vfs/zl_replace.h

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_SRC_FILES 	+= $(wildcard $(MY_MOAI_ROOT)/src/moai-android/*.cpp)

	include $(BUILD_STATIC_LIBRARY)
