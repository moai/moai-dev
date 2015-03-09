#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= moai-luaext
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
	LOCAL_CFLAGS		:= $(MY_LOCAL_CFLAGS) -include $(MOAI_SDK_HOME)/src/zl-vfs/zl_replace.h -fvisibility=hidden

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/luacrypto-0.3.2/src/lcrypto.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/luacurl-1.2.1/luacurl.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/luafilesystem-1.5.0/src/lfs.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/luasocket-2.0.2/src/auxiliar.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/luasocket-2.0.2/src/buffer.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/luasocket-2.0.2/src/except.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/luasocket-2.0.2/src/inet.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/luasocket-2.0.2/src/io.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/luasocket-2.0.2/src/luasocket.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/luasocket-2.0.2/src/mime.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/luasocket-2.0.2/src/options.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/luasocket-2.0.2/src/select.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/luasocket-2.0.2/src/tcp.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/luasocket-2.0.2/src/timeout.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/luasocket-2.0.2/src/udp.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/luasocket-2.0.2/src/unix.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/luasocket-2.0.2/src/usocket.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/luasocket-2.0.2-embed/fullluasocket.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/luasocket-2.0.2-embed/luasocketscripts.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/luasql-2.2.0/src/luasql.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/luasql-2.2.0/src/ls_sqlite3.c
	LOCAL_SRC_FILES   += $(wildcard $(MOAI_SDK_HOME)/src/moai-luaext/*.cpp) 

	include $(BUILD_STATIC_LIBRARY)