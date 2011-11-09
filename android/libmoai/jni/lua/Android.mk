#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= lua
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/lapi.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/lauxlib.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/lbaselib.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/lcode.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/ldblib.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/ldebug.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/ldo.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/ldump.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/lfunc.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/lgc.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/linit.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/liolib.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/lmathlib.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/lmem.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/loadlib.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/lobject.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/lopcodes.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/loslib.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/lparser.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/lstate.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/lstring.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/lstrlib.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/ltable.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/ltablib.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/ltm.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/lundump.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/lvm.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/lzio.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/print.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/moai-lua-5.1.3/src/llex.c

	include $(BUILD_STATIC_LIBRARY)
