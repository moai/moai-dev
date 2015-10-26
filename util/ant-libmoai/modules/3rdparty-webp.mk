#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

include $(CLEAR_VARS)

LOCAL_MODULE 		:= webp
LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
LOCAL_CFLAGS		:= -include $(MOAI_SDK_HOME)/src/zl-vfs/zl_replace.h
LOCAL_CFLAGS		+= -fvisibility=hidden

ifeq ($(TARGET_ARCH_ABI),$(filter $(TARGET_ARCH_ABI), armeabi-v7a x86))
	LOCAL_ARM_NEON  := true
endif # TARGET_ARCH_ABI == armeabi-v7a || x86

LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
LOCAL_SRC_FILES 	+= $(wildcard $(MOAI_SDK_HOME)/3rdparty/libwebp-0.4.1/src/dec/*.c)
LOCAL_SRC_FILES 	+= $(wildcard $(MOAI_SDK_HOME)/3rdparty/libwebp-0.4.1/src/demux/*.c)
LOCAL_SRC_FILES 	+= $(wildcard $(MOAI_SDK_HOME)/3rdparty/libwebp-0.4.1/src/dsp/*.c)
LOCAL_SRC_FILES 	+= $(wildcard $(MOAI_SDK_HOME)/3rdparty/libwebp-0.4.1/src/enc/*.c)
LOCAL_SRC_FILES 	+= $(wildcard $(MOAI_SDK_HOME)/3rdparty/libwebp-0.4.1/src/mux/*.c)
LOCAL_SRC_FILES 	+= $(wildcard $(MOAI_SDK_HOME)/3rdparty/libwebp-0.4.1/src/utils/*.c)
LOCAL_SRC_FILES 	+= $(wildcard $(MOAI_SDK_HOME)/3rdparty/libwebp-0.4.1/src/webp/*.c)

include $(BUILD_STATIC_LIBRARY)
