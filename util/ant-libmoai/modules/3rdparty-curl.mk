#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

include $(CLEAR_VARS)

LOCAL_MODULE 			:= curl
LOCAL_ARM_MODE 			:= $(MY_ARM_MODE)
LOCAL_CFLAGS 			:= -DHAVE_CONFIG_H  -DCURL_STATICLIB -DUSE_SSL -DUSE_MBEDTLS  -DBUILDING_LIBCURL -DUSE_ARES -DCARES_STATICLIB -DCURL_DISABLE_LDAP -DCURL_DISABLE_NTLM  -libcares
LOCAL_CFLAGS			+= -include $(MOAI_SDK_HOME)/src/zl-vfs/zl_replace.h
LOCAL_CFLAGS			+= -fvisibility=hidden

LOCAL_C_INCLUDES 		:= $(MY_HEADER_SEARCH_PATHS)
LOCAL_C_INCLUDES		+= $(MOAI_SDK_HOME)/src/custom-include/curl/android
LOCAL_C_INCLUDES		+= $(MOAI_SDK_HOME)/3rdparty/curl/lib

LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/amigaos.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/asyn-ares.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/asyn-thread.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/base64.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/conncache.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/connect.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/content_encoding.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/cookie.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/curl_addrinfo.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/curl_des.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/curl_endian.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/curl_fnmatch.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/curl_gethostname.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/curl_gssapi.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/curl_memrchr.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/curl_multibyte.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/curl_ntlm_core.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/curl_ntlm_wb.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/curl_rtmp.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/curl_sasl.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/curl_sspi.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/curl_threads.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/dict.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/dotdot.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/easy.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/escape.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/file.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/fileinfo.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/formdata.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/ftp.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/ftplistparser.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/getenv.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/getinfo.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/gopher.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/hash.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/hmac.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/hostasyn.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/hostcheck.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/hostip.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/hostip4.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/hostip6.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/hostsyn.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/http.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/http2.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/http_chunks.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/http_digest.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/http_negotiate.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/http_ntlm.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/http_proxy.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/idn_win32.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/if2ip.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/imap.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/inet_ntop.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/inet_pton.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/krb5.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/ldap.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/llist.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/md4.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/md5.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/memdebug.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/mprintf.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/multi.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/netrc.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/non-ascii.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/nonblock.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/nwlib.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/nwos.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/openldap.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/parsedate.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/pingpong.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/pipeline.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/pop3.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/progress.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/rand.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/rtsp.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/security.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/select.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/sendf.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/share.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/slist.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/smb.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/smtp.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/socks.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/socks_gssapi.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/socks_sspi.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/speedcheck.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/splay.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/ssh.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/strcase.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/strdup.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/strerror.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/strtok.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/strtoofft.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/system_win32.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/telnet.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/tftp.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/timeval.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/transfer.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/url.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/version.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/warnless.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/wildcard.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/x509asn1.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/vauth/cleartext.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/vauth/cram.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/vauth/digest.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/vauth/digest_sspi.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/vauth/krb5_gssapi.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/vauth/krb5_sspi.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/vauth/ntlm.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/vauth/ntlm_sspi.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/vauth/oauth2.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/vauth/spnego_gssapi.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/vauth/spnego_sspi.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/vauth/vauth.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/vtls/axtls.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/vtls/cyassl.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/vtls/darwinssl.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/vtls/gskit.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/vtls/gtls.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/vtls/mbedtls.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/vtls/nss.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/vtls/openssl.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/vtls/polarssl.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/vtls/polarssl_threadlock.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/vtls/schannel.c
LOCAL_SRC_FILES			+= $(MOAI_SDK_HOME)/3rdparty/curl/lib/vtls/vtls.c


include $(BUILD_STATIC_LIBRARY) 
