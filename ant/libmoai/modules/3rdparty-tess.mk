#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= tess
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
	LOCAL_CFLAGS		:= -include $(MOAI_SDK_HOME)/src/zl-vfs/zl_replace.h
	LOCAL_CFLAGS		+= -fvisibility=hidden

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libtess2/Source/bucketalloc.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libtess2/Source/dict.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libtess2/Source/geom.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libtess2/Source/mesh.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libtess2/Source/priorityq.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libtess2/Source/sweep.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/libtess2/Source/tess.c

	include $(BUILD_STATIC_LIBRARY)