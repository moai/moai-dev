#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	#================================================================#
	#temp fix should use line below just el capitan generate incorrect list of abi
	#APP_ABI 			:= @MY_ARM_ARCH@  
	#================================================================#
	APP_ABI				:= armeabi-v7a x86
	APP_CFLAGS			:= -w -DANDROID_NDK -DDISABLE_IMPORTGL
	APP_PLATFORM 		:= @MY_APP_PLATFORM@
	APP_STL 			:= gnustl_static
