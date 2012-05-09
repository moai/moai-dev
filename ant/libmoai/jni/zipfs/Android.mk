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
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/tlsf-2.0/tlsf.c

	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/zlib-1.2.3/adler32.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/zlib-1.2.3/compress.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/zlib-1.2.3/crc32.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/zlib-1.2.3/deflate.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/zlib-1.2.3/gzio.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/zlib-1.2.3/infback.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/zlib-1.2.3/inffast.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/zlib-1.2.3/inflate.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/zlib-1.2.3/inftrees.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/zlib-1.2.3/trees.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/zlib-1.2.3/uncompr.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/zlib-1.2.3/zutil.c

	include $(BUILD_STATIC_LIBRARY)