#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= cares
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
	LOCAL_CFLAGS		:= -include $(MOAI_SDK_HOME)/src/zl-vfs/zl_replace.h -DCARES_STATICLIB
	LOCAL_CFLAGS		+= -fvisibility=hidden

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares__close_sockets.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares__get_hostent.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares__read_line.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares__timeval.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_cancel.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_data.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_destroy.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_expand_name.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_expand_string.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_fds.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_free_hostent.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_free_string.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_getenv.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_gethostbyaddr.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_gethostbyname.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_getnameinfo.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_getopt.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_getsock.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_init.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_library_init.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_llist.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_mkquery.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_nowarn.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_options.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_parse_a_reply.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_parse_aaaa_reply.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_parse_mx_reply.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_parse_ns_reply.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_parse_ptr_reply.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_parse_srv_reply.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_parse_txt_reply.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_platform.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_process.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_query.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_search.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_send.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_strcasecmp.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_strdup.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_strerror.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_timeout.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_version.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_writev.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/bitncmp.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/inet_net_pton.c
	LOCAL_SRC_FILES 	+= $(MOAI_SDK_HOME)/3rdparty/c-ares-1.7.5/ares_inet_ntop.c

	include $(BUILD_STATIC_LIBRARY)
	
	