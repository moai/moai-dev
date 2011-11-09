#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= moaiext-luaext
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)

	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/luacrypto-0.2.0/src/lcrypto.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/luacurl-1.2.1/luacurl.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/luasocket-2.0.2/src/auxiliar.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/luasocket-2.0.2/src/buffer.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/luasocket-2.0.2/src/except.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/luasocket-2.0.2/src/inet.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/luasocket-2.0.2/src/io.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/luasocket-2.0.2/src/luasocket.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/luasocket-2.0.2/src/mime.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/luasocket-2.0.2/src/options.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/luasocket-2.0.2/src/select.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/luasocket-2.0.2/src/tcp.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/luasocket-2.0.2/src/timeout.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/luasocket-2.0.2/src/udp.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/luasocket-2.0.2/src/unix.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/luasocket-2.0.2/src/usocket.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/luasql-2.2.0/src/luasql.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/luasql-2.2.0/src/ls_sqlite3.c

	include $(BUILD_STATIC_LIBRARY)