#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include AppPlatformDefined.mk

	APP_ABI 		:= armeabi
	APP_CFLAGS		:= -w -DANDROID_NDK -DDISABLE_IMPORTGL
	APP_PLATFORM 	:= $(MY_APP_PLATFORM)
	APP_STL 		:= gnustl_static
