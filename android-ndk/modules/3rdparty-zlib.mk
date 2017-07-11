#================================================================#
# Copyright (c) 2010-2017 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

include $(CLEAR_VARS)

LOCAL_MODULE 		:= zlib
LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
LOCAL_CFLAGS		:= -fvisibility=hidden

LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)

LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/zlib-1.2.11/adler32.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/zlib-1.2.11/compress.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/zlib-1.2.11/crc32.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/zlib-1.2.11/deflate.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/zlib-1.2.11/gzclose.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/zlib-1.2.11/gzlib.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/zlib-1.2.11/gzread.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/zlib-1.2.11/gzwrite.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/zlib-1.2.11/infback.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/zlib-1.2.11/inffast.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/zlib-1.2.11/inflate.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/zlib-1.2.11/inftrees.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/zlib-1.2.11/trees.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/zlib-1.2.11/uncompr.c
LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/zlib-1.2.11/zutil.c

include $(BUILD_STATIC_LIBRARY)
