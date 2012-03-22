#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)
	
	include OptionalComponentsDefined.mk

	LOCAL_MODULE 		:= aku
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
	LOCAL_CFLAGS		:= $(DISABLE_TAPJOY) $(DISABLE_NOTIFICATIONS) $(DISABLE_BILLING) $(DISABLE_CRITTERCISM) $(DISABLE_ADCOLONY)
	LOCAL_CFLAGS		+= -include $(MY_MOAI_ROOT)/src/zipfs/zipfs_replace.h

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)	
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/aku/pch.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/aku/AKU.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/aku/AKU-luaext.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/aku/AKU-untz.cpp

	include $(BUILD_STATIC_LIBRARY)