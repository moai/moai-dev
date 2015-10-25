#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= crypto-b
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
	LOCAL_CFLAGS 		:= $(MY_CFLAGS) -w -DL_ENDIAN -DOPENSSL_NO_GMP -DOPENSSL_NO_JPAKE -DOPENSSL_NO_MD2 -DOPENSSL_NO_RC5 -DOPENSSL_NO_RFC3779 -DOPENSSL_NO_STORE
	LOCAL_CFLAGS		+= -include $(MOAI_SDK_HOME)/src/zl-vfs/zl_replace.h
	LOCAL_CFLAGS		+= -fvisibility=hidden

	LOCAL_C_INCLUDES 	+= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_C_INCLUDES 	+= '$(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/asn1'
	LOCAL_C_INCLUDES 	+= '$(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/evp'

	#Name="bn"
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_add.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_asm.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_blind.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_const.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_ctx.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_depr.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_div.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_err.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_exp.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_exp2.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_gcd.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_gf2m.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_kron.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_lib.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_mod.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_mont.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_mpi.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_mul.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_nist.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_prime.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_print.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_rand.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_recp.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_shift.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_sqr.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_sqrt.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bn/bn_word.c

	#Name="ec"
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ec/ec2_mult.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ec/ec2_smpl.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ec/ec_ameth.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ec/ec_asn1.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ec/ec_check.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ec/ec_curve.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ec/ec_cvt.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ec/ec_err.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ec/ec_key.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ec/ec_lib.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ec/ec_mult.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ec/ec_pmeth.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ec/ec_print.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ec/eck_prn.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ec/ecp_mont.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ec/ecp_nist.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ec/ecp_smpl.c

	#Name="hmac"
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/hmac/hm_ameth.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/hmac/hm_pmeth.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/hmac/hmac.c

	#Name="pem"
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pem/pem_all.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pem/pem_err.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pem/pem_info.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pem/pem_lib.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pem/pem_oth.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pem/pem_pk8.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pem/pem_pkey.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pem/pem_seal.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pem/pem_sign.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pem/pem_x509.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pem/pem_xaux.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pem/pvkfmt.c

	#Name="dsa"
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/dsa/dsa_ameth.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/dsa/dsa_asn1.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/dsa/dsa_depr.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/dsa/dsa_err.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/dsa/dsa_gen.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/dsa/dsa_key.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/dsa/dsa_lib.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/dsa/dsa_ossl.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/dsa/dsa_pmeth.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/dsa/dsa_prn.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/dsa/dsa_sign.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/dsa/dsa_vrf.c

	#Name="dh"
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/dh/dh_ameth.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/dh/dh_asn1.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/dh/dh_check.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/dh/dh_depr.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/dh/dh_err.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/dh/dh_gen.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/dh/dh_key.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/dh/dh_lib.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/dh/dh_pmeth.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/dh/dh_prn.c

	#Name="bf"
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bf/bf_cfb64.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bf/bf_ecb.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bf/bf_enc.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bf/bf_ofb64.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bf/bf_skey.c

	#Name="cast"
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/cast/c_cfb64.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/cast/c_ecb.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/cast/c_enc.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/cast/c_ofb64.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/cast/c_skey.c

	#Name="mdc2"
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/mdc2/mdc2_one.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/mdc2/mdc2dgst.c

	#Name="ripemd"
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ripemd/rmd_dgst.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ripemd/rmd_one.c

	#Name="whirlpool"
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/whrlpool/wp_block.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/whrlpool/wp_dgst.c

	#Name="modes"
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/modes/cbc128.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/modes/cfb128.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/modes/ctr128.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/modes/cts128.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/modes/ofb128.c

	#Name="ecdh"
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ecdh/ech_err.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ecdh/ech_key.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ecdh/ech_lib.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ecdh/ech_ossl.c

	#Name="pkcs7"
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pkcs7/bio_pk7.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pkcs7/pk7_asn1.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pkcs7/pk7_attr.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pkcs7/pk7_dgst.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pkcs7/pk7_doit.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pkcs7/pk7_lib.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pkcs7/pk7_mime.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pkcs7/pk7_smime.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pkcs7/pkcs7err.c

	#Name="pkcs12"
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pkcs12/p12_add.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pkcs12/p12_asn.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pkcs12/p12_attr.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pkcs12/p12_crpt.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pkcs12/p12_crt.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pkcs12/p12_decr.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pkcs12/p12_init.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pkcs12/p12_key.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pkcs12/p12_kiss.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pkcs12/p12_mutl.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pkcs12/p12_npas.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pkcs12/p12_p8d.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pkcs12/p12_p8e.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pkcs12/p12_utl.c
	LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pkcs12/pk12err.c

	include $(BUILD_STATIC_LIBRARY)