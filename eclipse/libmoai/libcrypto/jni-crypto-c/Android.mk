#================================================================#
# libcrypto-c
#================================================================#
include $(CLEAR_VARS)

LOCAL_CFLAGS = -DL_ENDIAN -DOPENSSL_NO_GMP -DOPENSSL_NO_JPAKE -DOPENSSL_NO_MD2 -DOPENSSL_NO_RC5 -DOPENSSL_NO_RFC3779 -DOPENSSL_NO_STORE
LOCAL_MODULE := libcrypto-c

#----------------------------------------------------------------#
# header search paths
#----------------------------------------------------------------#
LOCAL_C_INCLUDES := $(SRC_PATH)/3rdparty/openssl-1.0.0d/include-android

#----------------------------------------------------------------#
# included files
#----------------------------------------------------------------#
#Name="aes"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/aes/aes_cbc.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/aes/aes_cfb.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/aes/aes_core.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/aes/aes_ctr.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/aes/aes_ecb.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/aes/aes_ige.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/aes/aes_misc.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/aes/aes_ofb.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/aes/aes_wrap.c

#Name="bio"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bio/b_dump.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bio/b_print.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bio/b_sock.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bio/bf_buff.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bio/bf_lbuf.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bio/bf_nbio.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bio/bf_null.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bio/bio_cb.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bio/bio_err.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bio/bio_lib.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bio/bss_acpt.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bio/bss_bio.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bio/bss_conn.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bio/bss_dgram.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bio/bss_fd.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bio/bss_file.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bio/bss_log.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bio/bss_mem.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bio/bss_null.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bio/bss_sock.c

#Name="buffer"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/buffer/buf_err.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/buffer/buffer.c

#Name="camellia"

#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/camellia/camellia.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/camellia/cmll_cbc.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/camellia/cmll_cfb.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/camellia/cmll_ctr.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/camellia/cmll_ecb.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/camellia/cmll_misc.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/camellia/cmll_ofb.c

#Name="des"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/des/cbc_cksm.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/des/cbc_enc.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/des/cfb64ede.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/des/cfb64enc.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/des/cfb_enc.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/des/des_enc.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/des/ecb3_enc.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/des/ecb_enc.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/des/enc_read.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/des/enc_writ.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/des/fcrypt.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/des/fcrypt_b.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/des/ofb64ede.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/des/ofb64enc.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/des/ofb_enc.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/des/pcbc_enc.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/des/qud_cksm.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/des/rand_key.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/des/read2pwd.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/des/rpc_enc.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/des/set_key.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/des/str2key.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/des/xcbc_enc.c

#Name="ecdsa"

#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ecdsa/ecs_asn1.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ecdsa/ecs_err.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ecdsa/ecs_lib.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ecdsa/ecs_ossl.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ecdsa/ecs_sign.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ecdsa/ecs_vrf.c

#Name="err"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/err/err.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/err/err_all.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/err/err_prn.c

#Name="comp"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/comp/c_rle.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/comp/c_zlib.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/comp/comp_err.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/comp/comp_lib.c

#Name="idea"

#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/idea/i_cbc.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/idea/i_cfb64.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/idea/i_ecb.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/idea/i_ofb64.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/idea/i_skey.c

#Name="objects"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/objects/o_names.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/objects/obj_dat.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/objects/obj_err.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/objects/obj_lib.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/objects/obj_xref.c

#Name="rc2"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rc2/rc2_cbc.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rc2/rc2_ecb.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rc2/rc2_skey.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rc2/rc2cfb64.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rc2/rc2ofb64.c

#Name="rc4"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rc4/rc4_enc.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rc4/rc4_skey.c

#Name="md5"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/md5/md5_dgst.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/md5/md5_one.c

#Name="sha"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/sha/sha1_one.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/sha/sha1dgst.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/sha/sha256.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/sha/sha512.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/sha/sha_dgst.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/sha/sha_one.c

#Name="seed"

#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/seed/seed.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/seed/seed_cbc.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/seed/seed_cfb.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/seed/seed_ecb.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/seed/seed_ofb.c

#Name="md4"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/md4/md4_dgst.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/md4/md4_one.c

#Name="pqueue"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pqueue/pqueue.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/cpt_err.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/cryptlib.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/cversion.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ebcdic.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ex_data.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/mem.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/mem_clr.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/mem_dbg.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/o_dir.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/o_str.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/o_time.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/uid.c

#Name="stack"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/stack/stack.c

#Name="ui"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ui/ui_err.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ui/ui_lib.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ui/ui_openssl.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ui/ui_util.c

#Name="x509"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509/by_dir.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509/by_file.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509/x509_att.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509/x509_cmp.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509/x509_d2.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509/x509_def.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509/x509_err.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509/x509_ext.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509/x509_lu.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509/x509_obj.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509/x509_r2x.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509/x509_req.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509/x509_set.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509/x509_trs.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509/x509_txt.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509/x509_v3.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509/x509_vfy.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509/x509_vpm.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509/x509cset.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509/x509name.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509/x509rset.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509/x509spki.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509/x509type.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509/x_all.c

#Name="x509v3"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/pcy_cache.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/pcy_data.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/pcy_lib.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/pcy_map.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/pcy_node.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/pcy_tree.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_addr.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_akey.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_akeya.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_alt.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_asid.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_bcons.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_bitst.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_conf.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_cpols.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_crld.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_enum.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_extku.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_genn.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_ia5.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_info.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_int.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_lib.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_ncons.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_ocsp.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_pci.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_pcia.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_pcons.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_pku.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_pmaps.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_prn.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_purp.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_skey.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_sxnet.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_utl.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3err.c

include $(BUILD_STATIC_LIBRARY)
