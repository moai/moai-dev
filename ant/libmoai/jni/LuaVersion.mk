#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	LUA_VERSION := $(strip @LUA_VERSION@)	
	LUA_VERSION := $(strip $(LUA_VERSION))
	
  ifeq ($(LUA_VERSION),5.1)
    LUA_COMPAT_MODULE :=
	else
  ifeq ($(LUA_VERSION),5.2)
    LUA_COMPAT_MODULE := -DLUA_COMPAT_MODULE
	else
    $(error Invalid Lua version $(LUA_VERSION). Supported versions are 5.1 and 5.2)
	endif
	endif