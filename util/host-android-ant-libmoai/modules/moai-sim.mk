#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= moai-sim
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
	LOCAL_CFLAGS		:= $(MY_LOCAL_CFLAGS) -DMOAI_WITH_LIBWEBP=0 -DMOAI_WITH_LIBPVR=1 -include $(MOAI_SDK_HOME)/src/zl-vfs/zl_replace.h -fvisibility=hidden

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_SRC_FILES 	+= $(wildcard $(MOAI_SDK_HOME)/src/moai-sim/*.cpp)

	include $(BUILD_STATIC_LIBRARY)
