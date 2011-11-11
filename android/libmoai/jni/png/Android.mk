#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= png
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lpng140/png.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lpng140/pngerror.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lpng140/pngget.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lpng140/pngmem.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lpng140/pngpread.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lpng140/pngread.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lpng140/pngrio.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lpng140/pngrtran.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lpng140/pngrutil.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lpng140/pngset.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lpng140/pngtest.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lpng140/pngtrans.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lpng140/pngwio.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lpng140/pngwrite.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lpng140/pngwtran.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/lpng140/pngwutil.c

	include $(BUILD_STATIC_LIBRARY)