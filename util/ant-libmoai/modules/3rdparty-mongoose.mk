#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

include $(CLEAR_VARS)

LOCAL_MODULE 		:= mongoose
LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
LOCAL_CFLAGS		:= -fvisibility=hidden

LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)

LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/mongoose/mongoose.c

include $(BUILD_STATIC_LIBRARY)
