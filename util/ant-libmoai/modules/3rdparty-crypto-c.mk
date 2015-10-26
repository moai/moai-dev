#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

include $(CLEAR_VARS)

LOCAL_MODULE 		:= crypto-c
LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
LOCAL_CFLAGS 		:= $(MY_CFLAGS) -w -DL_ENDIAN -DOPENSSL_NO_GMP -DOPENSSL_NO_JPAKE -DOPENSSL_NO_MD2 -DOPENSSL_NO_RC5 -DOPENSSL_NO_RFC3779 -DOPENSSL_NO_STORE
LOCAL_CFLAGS		+= -include $(MOAI_SDK_HOME)/src/zl-vfs/zl_replace.h
LOCAL_CFLAGS		+= -fvisibility=hidden

LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)

#Name="aes"
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/aes/aes_cbc.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/aes/aes_cfb.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/aes/aes_core.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/aes/aes_ctr.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/aes/aes_ecb.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/aes/aes_ige.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/aes/aes_misc.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/aes/aes_ofb.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/aes/aes_wrap.c

#Name="bio"
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bio/b_dump.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bio/b_print.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bio/b_sock.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bio/bf_buff.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bio/bf_lbuf.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bio/bf_nbio.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bio/bf_null.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bio/bio_cb.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bio/bio_err.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bio/bio_lib.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bio/bss_acpt.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bio/bss_bio.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bio/bss_conn.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bio/bss_dgram.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bio/bss_fd.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bio/bss_file.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bio/bss_log.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bio/bss_mem.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bio/bss_null.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/bio/bss_sock.c

#Name="buffer"
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/buffer/buf_err.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/buffer/buffer.c

#Name="camellia"
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/camellia/camellia.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/camellia/cmll_cbc.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/camellia/cmll_cfb.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/camellia/cmll_ctr.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/camellia/cmll_ecb.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/camellia/cmll_misc.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/camellia/cmll_ofb.c

#Name="des"
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/des/cbc_cksm.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/des/cbc_enc.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/des/cfb64ede.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/des/cfb64enc.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/des/cfb_enc.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/des/des_enc.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/des/ecb3_enc.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/des/ecb_enc.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/des/enc_read.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/des/enc_writ.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/des/fcrypt.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/des/fcrypt_b.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/des/ofb64ede.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/des/ofb64enc.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/des/ofb_enc.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/des/pcbc_enc.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/des/qud_cksm.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/des/rand_key.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/des/read2pwd.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/des/rpc_enc.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/des/set_key.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/des/str2key.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/des/xcbc_enc.c

#Name="ecdsa"
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ecdsa/ecs_asn1.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ecdsa/ecs_err.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ecdsa/ecs_lib.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ecdsa/ecs_ossl.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ecdsa/ecs_sign.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ecdsa/ecs_vrf.c

#Name="err"
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/err/err.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/err/err_all.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/err/err_prn.c

#Name="comp"
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/comp/c_rle.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/comp/c_zlib.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/comp/comp_err.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/comp/comp_lib.c

#Name="idea"
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/idea/i_cbc.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/idea/i_cfb64.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/idea/i_ecb.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/idea/i_ofb64.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/idea/i_skey.c

#Name="objects"
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/objects/o_names.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/objects/obj_dat.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/objects/obj_err.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/objects/obj_lib.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/objects/obj_xref.c

#Name="rc2"
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/rc2/rc2_cbc.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/rc2/rc2_ecb.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/rc2/rc2_skey.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/rc2/rc2cfb64.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/rc2/rc2ofb64.c

#Name="rc4"
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/rc4/rc4_enc.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/rc4/rc4_skey.c

#Name="md5"
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/md5/md5_dgst.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/md5/md5_one.c

#Name="sha"
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/sha/sha1_one.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/sha/sha1dgst.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/sha/sha256.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/sha/sha512.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/sha/sha_dgst.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/sha/sha_one.c

#Name="seed"
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/seed/seed.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/seed/seed_cbc.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/seed/seed_cfb.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/seed/seed_ecb.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/seed/seed_ofb.c

#Name="md4"
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/md4/md4_dgst.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/md4/md4_one.c

#Name="pqueue"
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/pqueue/pqueue.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/cpt_err.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/cryptlib.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/cversion.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ebcdic.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ex_data.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/mem.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/mem_clr.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/mem_dbg.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/o_dir.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/o_str.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/o_time.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/uid.c

#Name="stack"
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/stack/stack.c

#Name="ui"
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ui/ui_err.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ui/ui_lib.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ui/ui_openssl.c
LOCAL_SRC_FILES += $(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m/crypto/ui/ui_util.c

include $(BUILD_STATIC_LIBRARY)