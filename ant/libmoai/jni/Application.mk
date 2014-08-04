#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include ArmModeDefined.mk
	include AppPlatformDefined.mk

	APP_ABI 		:= $(MY_ARM_ARCH)
	APP_CFLAGS		:= -w -DANDROID_NDK -DDISABLE_IMPORTGL
	APP_PLATFORM 	:= $(MY_APP_PLATFORM)
	APP_STL 		:= gnustl_static
