#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= expat
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
	LOCAL_CFLAGS		:= -DHAVE_MEMMOVE=1

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/expat-2.0.1/lib/xmlparse.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/expat-2.0.1/lib/xmlrole.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/expat-2.0.1/lib/xmltok.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/expat-2.0.1/lib/xmltok_impl.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/expat-2.0.1/lib/xmltok_ns.c

	include $(BUILD_STATIC_LIBRARY)
	