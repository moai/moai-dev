#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	LOCAL_PATH := $(call my-dir)
	include $(CLEAR_VARS)

	include ../../../ArmModeDefined.mk
	
	MY_MOAI_ROOT 			:= ../../../../../..
	MY_HEADER_SEARCH_PATHS	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/include-android
	MY_CFLAGS				+= -w -DL_ENDIAN -DOPENSSL_NO_GMP -DOPENSSL_NO_JPAKE -DOPENSSL_NO_MD2 -DOPENSSL_NO_RC5 -DOPENSSL_NO_RFC3779 -DOPENSSL_NO_STORE

	include crypto-a/Android.mk
	include crypto-b/Android.mk
	include crypto-c/Android.mk
	include crypto-d/Android.mk
