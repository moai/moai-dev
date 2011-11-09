#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)
	
	LOCAL_MODULE 			:= curl
	LOCAL_ARM_MODE 			:= $(MY_ARM_MODE)
	LOCAL_CFLAGS 			:= -DHAVE_CONFIG_H -DUSE_SSLEAY -DUSE_OPENSSL

	LOCAL_C_INCLUDES 		:= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/file.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/timeval.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/base64.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/hostip.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/progress.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/formdata.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/cookie.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/sendf.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/ftp.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/url.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/dict.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/if2ip.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/speedcheck.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/ldap.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/ssluse.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/version.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/getenv.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/escape.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/mprintf.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/telnet.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/netrc.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/getinfo.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/transfer.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/strequal.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/easy.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/security.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/krb4.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/krb5.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/memdebug.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/http_chunks.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/strtok.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/connect.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/llist.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/hash.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/multi.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/http.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/content_encoding.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/share.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/http_digest.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/md5.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/curl_rand.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/http_negotiate.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/http_ntlm.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/inet_pton.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/strtoofft.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/strerror.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/hostares.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/hostasyn.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/hostip4.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/hostip6.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/hostsyn.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/hostthre.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/inet_ntop.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/parsedate.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/select.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/gtls.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/sslgen.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/tftp.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/splay.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/strdup.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/socks.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/ssh.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/nss.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/qssl.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/rawstr.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/curl_addrinfo.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/socks_gssapi.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/socks_sspi.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/curl_sspi.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/slist.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/nonblock.c
	LOCAL_SRC_FILES			+= $(MY_MOAI_ROOT)/3rdparty/curl-7.19.7/lib/curl_memrchr.c

	include $(BUILD_STATIC_LIBRARY) 
