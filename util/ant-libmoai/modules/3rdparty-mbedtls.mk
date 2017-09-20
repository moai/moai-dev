#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

include $(CLEAR_VARS)

LOCAL_MODULE 		:= mbedtls
LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
LOCAL_CFLAGS		:= -DMBEDTLS_EXPORTS
LOCAL_CFLAGS		+= -include $(MOAI_SDK_HOME)/src/zl-vfs/zl_replace.h
LOCAL_CFLAGS		+= -fvisibility=hidden

LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)

LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/aes.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/aesni.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/arc4.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/asn1parse.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/asn1write.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/base64.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/bignum.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/blowfish.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/camellia.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/ccm.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/certs.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/cipher.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/cipher_wrap.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/cmac.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/ctr_drbg.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/debug.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/des.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/dhm.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/ecdh.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/ecdsa.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/ecjpake.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/ecp.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/ecp_curves.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/entropy.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/entropy_poll.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/error.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/gcm.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/havege.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/hmac_drbg.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/md.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/md2.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/md4.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/md5.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/md_wrap.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/memory_buffer_alloc.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/net_sockets.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/oid.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/padlock.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/pem.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/pk.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/pkcs11.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/pkcs12.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/pkcs5.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/pkparse.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/pkwrite.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/pk_wrap.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/platform.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/ripemd160.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/rsa.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/sha1.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/sha256.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/sha512.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/ssl_cache.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/ssl_ciphersuites.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/ssl_cli.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/ssl_cookie.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/ssl_srv.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/ssl_ticket.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/ssl_tls.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/threading.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/timing.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/version.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/version_features.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/x509.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/x509write_crt.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/x509write_csr.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/x509_create.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/x509_crl.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/x509_crt.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/x509_csr.c
LOCAL_SRC_FILES		+= $(MOAI_SDK_HOME)/3rdparty/mbedtls/library/xtea.c


include $(BUILD_STATIC_LIBRARY)
