#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= ssl
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
	LOCAL_CFLAGS 		:= -DOPENSSL_NO_DEPRECATED

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/bio_ssl.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/d1_both.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/d1_clnt.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/d1_enc.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/d1_lib.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/d1_meth.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/d1_pkt.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/d1_srvr.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/kssl.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/s23_clnt.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/s23_lib.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/s23_meth.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/s23_pkt.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/s23_srvr.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/s2_clnt.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/s2_enc.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/s2_lib.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/s2_meth.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/s2_pkt.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/s2_srvr.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/s3_both.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/s3_clnt.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/s3_enc.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/s3_lib.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/s3_meth.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/s3_pkt.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/s3_srvr.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/ssl_algs.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/ssl_asn1.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/ssl_cert.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/ssl_ciph.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/ssl_err.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/ssl_err2.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/ssl_lib.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/ssl_rsa.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/ssl_sess.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/ssl_stat.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/ssl_txt.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/t1_clnt.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/t1_enc.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/t1_lib.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/t1_meth.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/t1_reneg.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/ssl/t1_srvr.c
	
	include $(BUILD_STATIC_LIBRARY)