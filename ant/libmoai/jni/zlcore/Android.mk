#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= zlcore
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/zlcore/zlcore-pch.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/zlcore/zl_mutex.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/zlcore/zl_util.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/zlcore/zl_vfscanf.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/zlcore/zlcore.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/zlcore/ZLDirectoryItr.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/zlcore/ZLFile.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/zlcore/ZLFileSystem.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/zlcore/ZLVirtualPath.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/zlcore/ZLZipArchive.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/src/zlcore/ZLZipStream.cpp
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