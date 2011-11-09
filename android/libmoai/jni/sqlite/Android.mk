#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= sqlite
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
	LOCAL_CFLAGS 		:= -w -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -DSQLITE_ENABLE_COLUMN_METADATA
	LOCAL_CFLAGS 		+= -D_UNICODE -DUNICODE -D_LIB -DNDEBUG -Dfdatasync=fsync

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/sqlite-3.6.16/sqlite3.c

	include $(BUILD_STATIC_LIBRARY)