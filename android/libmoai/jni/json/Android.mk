#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= json
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jansson-2.1/src/dump.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jansson-2.1/src/error.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jansson-2.1/src/hashtable.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jansson-2.1/src/load.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jansson-2.1/src/memory.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jansson-2.1/src/pack_unpack.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jansson-2.1/src/strbuffer.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jansson-2.1/src/utf.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jansson-2.1/src/value.c

	include $(BUILD_STATIC_LIBRARY)
