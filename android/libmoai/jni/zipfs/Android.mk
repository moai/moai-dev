#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= zipfs
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/zipfs/zipfs-pch.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/zipfs/zipfs.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/zipfs/zipfs_util.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/zipfs/zipfs_vfscanf.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/zipfs/ZIPFSString.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/zipfs/ZIPFSVirtualPath.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/zipfs/ZIPFSZipFile.c

	include $(BUILD_STATIC_LIBRARY)