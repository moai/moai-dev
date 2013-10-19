#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= moaicore
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
	LOCAL_CFLAGS		:= -include $(MY_MOAI_ROOT)/src/zl-vfs/zl_replace.h

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_SRC_FILES 	+= $(wildcard $(MY_MOAI_ROOT)/src/moai-core/*.cpp)
	LOCAL_SRC_FILES 	+= $(wildcard $(MY_MOAI_ROOT)/src/moai-crypto/*.cpp)
	LOCAL_SRC_FILES   += $(wildcard $(MY_MOAI_ROOT)/src/moai-util/*.cpp)
	LOCAL_SRC_FILES   += $(wildcard $(MY_MOAI_ROOT)/src/moai-sim/*.cpp)
	LOCAL_SRC_FILES   += $(wildcard $(MY_MOAI_ROOT)/src/moai-box2d/*.cpp)
	LOCAL_SRC_FILES   += $(wildcard $(MY_MOAI_ROOT)/src/moai-chipmunk/*.cpp)
	LOCAL_SRC_FILES   += $(wildcard $(MY_MOAI_ROOT)/src/moai-http-client/*.cpp)
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/host-modules/aku_modules.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/host-modules/aku_modules_util.cpp

	include $(BUILD_STATIC_LIBRARY)