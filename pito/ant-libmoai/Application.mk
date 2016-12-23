#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

    APP_ABI                 := @MY_ARM_ARCH@  
    APP_CFLAGS              := -w -DANDROID_NDK -DDISABLE_IMPORTGL -D__STDC_LIMIT_MACROS
    APP_PLATFORM            := @MY_APP_PLATFORM@
    APP_STL                 := gnustl_static
    NDK_TOOLCHAIN_VERSION   := 4.9