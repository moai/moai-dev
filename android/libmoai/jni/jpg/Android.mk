#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= jpg
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jaricom.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jcapimin.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jcapistd.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jcarith.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jccoefct.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jccolor.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jcdctmgr.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jchuff.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jcinit.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jcmainct.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jcmarker.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jcmaster.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jcomapi.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jcparam.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jcprepct.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jcsample.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jctrans.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jdapimin.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jdapistd.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jdarith.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jdatadst.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jdatasrc.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jdcoefct.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jdcolor.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jddctmgr.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jdhuff.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jdinput.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jdmainct.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jdmarker.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jdmaster.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jdmerge.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jdpostct.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jdsample.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jdtrans.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jerror-moai.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jfdctflt.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jfdctfst.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jfdctint.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jidctflt.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jidctfst.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jidctint.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jmemmgr.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jmemnobs.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jquant1.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jquant2.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/jpeg-8c/jutils.c

	include $(BUILD_STATIC_LIBRARY)
