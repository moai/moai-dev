#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

include $(CLEAR_VARS)

LOCAL_MODULE 		:= ogg
LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
LOCAL_CFLAGS		:= -include $(MOAI_SDK_HOME)/src/zl-vfs/zl_replace.h
LOCAL_CFLAGS		+= -fvisibility=hidden

LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libogg-1.2.2/src/bitwise.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libogg-1.2.2/src/framing.c

include $(BUILD_STATIC_LIBRARY)
