#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= lua
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
	LOCAL_CFLAGS		:= -include $(MY_MOAI_ROOT)/src/zlcore/zl_replace.h $(LUA_COMPAT_MODULE)

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)

    ifeq ($(LUA_VERSION),5.1)
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/lapi.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/lauxlib.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/lbaselib.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/lcode.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/ldblib.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/ldebug.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/ldo.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/ldump.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/lfunc.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/lgc.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/linit.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/liolib.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/android/llex.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/lmathlib.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/lmem.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/loadlib.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/lobject.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/lopcodes.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/loslib.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/lparser.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/lstate.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/lstring.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/lstrlib.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/ltable.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/ltablib.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/ltm.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/lundump.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/lvm.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/lzio.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.1.3/src/print.c
    else
    ifeq ($(LUA_VERSION),5.2)
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/lapi.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/lauxlib.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/lbaselib.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/lbitlib.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/lcode.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/lcorolib.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/lctype.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/ldblib.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/ldebug.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/ldo.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/ldump.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/lfunc.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/lgc.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/linit.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/liolib.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/android/llex.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/lmathlib.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/lmem.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/loadlib.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/lobject.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/lopcodes.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/loslib.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/lparser.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/lstate.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/lstring.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/lstrlib.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/ltable.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/ltablib.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/ltm.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/lundump.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/lvm.c
		LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lua-5.2.1/src/lzio.c
    else
        $(error Invalid Lua version $(LUA_VERSION). Supported versions are 5.1 and 5.2)
	endif
	endif

	include $(BUILD_STATIC_LIBRARY)
