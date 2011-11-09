#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	LOCAL_PATH := $(call my-dir)
	include $(CLEAR_VARS)

	include ../../../ArmModeDefined.mk
	
	LOCAL_MODULE 	:= crypto
	LOCAL_ARM_MODE 	:= $(MY_ARM_MODE)
	LOCAL_CFLAGS 	:= -w -DL_ENDIAN -DOPENSSL_NO_GMP -DOPENSSL_NO_JPAKE -DOPENSSL_NO_MD2 -DOPENSSL_NO_RC5 -DOPENSSL_NO_RFC3779 -DOPENSSL_NO_STORE
	
	MY_MOAI_ROOT 		:= ../../../../../..
	LOCAL_C_INCLUDES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/include-android

	include ../../lists/crypto-a.mk
	include ../../lists/crypto-b.mk
	include ../../lists/crypto-c.mk
	include ../../lists/crypto-d.mk
	
	include $(BUILD_STATIC_LIBRARY)