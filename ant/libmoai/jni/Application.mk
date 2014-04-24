#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include ArmModeDefined.mk
	include AppPlatformDefined.mk

	APP_ABI 		:= $(MY_ARM_ARCH)
	APP_CFLAGS		:= -w -DANDROID_NDK -DDISABLE_IMPORTGL -DUSE_CHIPMUNK=0
	APP_PLATFORM 	:= $(MY_APP_PLATFORM)
	APP_STL 		:= gnustl_static

	ifeq ($(NDK_DEBUG),1)
	    APP_OPTIM := debug
	    APP_CFLAGS += -g -O0
	else
	    APP_OPTIM := release
	    APP_CFLAGS += -O3
	endif