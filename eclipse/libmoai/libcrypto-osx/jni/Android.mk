LOCAL_PATH := $(call my-dir)
SRC_PATH := ../../../..

#================================================================#
# libcrypto
#================================================================#
include $(CLEAR_VARS)

LOCAL_CFLAGS = -DL_ENDIAN -DOPENSSL_NO_GMP -DOPENSSL_NO_JPAKE -DOPENSSL_NO_MD2 -DOPENSSL_NO_RC5 -DOPENSSL_NO_RFC3779 -DOPENSSL_NO_STORE
LOCAL_MODULE := libcrypto

#----------------------------------------------------------------#
# header search paths
#----------------------------------------------------------------#
LOCAL_C_INCLUDES := $(SRC_PATH)/3rdparty/openssl-1.0.0d/include-android

#----------------------------------------------------------------#
# included files
#----------------------------------------------------------------#
#Name="asn1"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/a_bitstr.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/a_bool.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/a_bytes.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/a_d2i_fp.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/a_digest.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/a_dup.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/a_enum.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/a_gentm.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/a_i2d_fp.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/a_int.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/a_mbstr.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/a_object.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/a_octet.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/a_print.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/a_set.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/a_sign.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/a_strex.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/a_strnid.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/a_time.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/a_type.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/a_utctm.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/a_utf8.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/a_verify.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/ameth_lib.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/asn1_err.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/asn1_gen.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/asn1_lib.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/asn1_par.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/asn_mime.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/asn_moid.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/asn_pack.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/bio_asn1.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/bio_ndef.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/d2i_pr.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/d2i_pu.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/evp_asn1.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/f_enum.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/f_int.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/f_string.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/i2d_pr.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/i2d_pu.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/n_pkey.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/nsseq.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/p5_pbe.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/p5_pbev2.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/p8_pkey.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/t_bitst.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/t_crl.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/t_pkey.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/t_req.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/t_spki.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/t_x509.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/t_x509a.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/tasn_dec.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/tasn_enc.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/tasn_fre.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/tasn_new.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/tasn_prn.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/tasn_typ.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/tasn_utl.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/x_algor.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/x_attrib.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/x_bignum.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/x_crl.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/x_exten.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/x_info.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/x_long.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/x_name.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/x_nx509.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/x_pkey.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/x_pubkey.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/x_req.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/x_sig.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/x_spki.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/x_val.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/x_x509.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/asn1/x_x509a.c

#Name="conf"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/conf/conf_api.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/conf/conf_def.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/conf/conf_err.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/conf/conf_lib.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/conf/conf_mall.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/conf/conf_mod.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/conf/conf_sap.c

#Name="engine"

#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/engine/eng_all.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/engine/eng_cnf.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/engine/eng_cryptodev.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/engine/eng_ctrl.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/engine/eng_dyn.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/engine/eng_err.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/engine/eng_fat.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/engine/eng_init.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/engine/eng_lib.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/engine/eng_list.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/engine/eng_openssl.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/engine/eng_pkey.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/engine/eng_table.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/engine/tb_asnmth.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/engine/tb_cipher.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/engine/tb_dh.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/engine/tb_digest.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/engine/tb_dsa.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/engine/tb_ecdh.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/engine/tb_ecdsa.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/engine/tb_pkmeth.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/engine/tb_rand.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/engine/tb_rsa.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/engine/tb_store.c

#Name="evp"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/bio_b64.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/bio_enc.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/bio_md.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/bio_ok.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/c_all.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/c_allc.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/c_alld.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/digest.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/e_aes.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/e_bf.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/e_camellia.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/e_cast.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/e_des.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/e_des3.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/e_idea.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/e_null.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/e_old.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/e_rc2.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/e_rc4.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/e_rc5.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/e_seed.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/e_xcbc_d.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/encode.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/evp_acnf.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/evp_enc.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/evp_err.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/evp_key.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/evp_lib.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/evp_pbe.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/evp_pkey.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/m_dss.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/m_dss1.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/m_ecdsa.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/m_md2.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/m_md4.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/m_md5.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/m_mdc2.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/m_null.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/m_ripemd.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/m_sha.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/m_sha1.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/m_sigver.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/m_wp.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/names.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/p5_crpt.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/p5_crpt2.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/p_dec.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/p_enc.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/p_lib.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/p_open.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/p_seal.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/p_sign.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/p_verify.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/pmeth_fn.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/pmeth_gn.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/evp/pmeth_lib.c

#Name="lhash"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/lhash/lh_stats.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/lhash/lhash.c

#Name="ocsp"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ocsp/ocsp_asn.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ocsp/ocsp_cl.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ocsp/ocsp_err.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ocsp/ocsp_ext.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ocsp/ocsp_ht.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ocsp/ocsp_lib.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ocsp/ocsp_prn.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ocsp/ocsp_srv.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ocsp/ocsp_vfy.c

#Name="rand"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rand/md_rand.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rand/rand_egd.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rand/rand_err.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rand/rand_lib.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rand/rand_nw.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rand/rand_os2.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rand/rand_unix.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rand/rand_win.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rand/randfile.c

#Name="rsa"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_ameth.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_asn1.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_chk.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_depr.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_eay.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_err.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_gen.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_lib.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_none.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_null.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_oaep.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_pk1.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_pmeth.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_prn.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_pss.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_saos.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_sign.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_ssl.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_x931.c

#Name="bn"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_add.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_asm.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_blind.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_const.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_ctx.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_depr.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_div.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_err.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_exp.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_exp2.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_gcd.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_gf2m.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_kron.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_lib.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_mod.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_mont.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_mpi.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_mul.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_nist.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_prime.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_print.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_rand.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_recp.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_shift.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_sqr.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_sqrt.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bn/bn_word.c

#Name="ec"

#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ec/ec2_mult.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ec/ec2_smpl.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ec/ec_ameth.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ec/ec_asn1.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ec/ec_check.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ec/ec_curve.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ec/ec_cvt.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ec/ec_err.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ec/ec_key.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ec/ec_lib.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ec/ec_mult.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ec/ec_pmeth.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ec/ec_print.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ec/eck_prn.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ec/ecp_mont.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ec/ecp_nist.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ec/ecp_smpl.c

#Name="hmac"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/hmac/hm_ameth.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/hmac/hm_pmeth.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/hmac/hmac.c

#Name="pem"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pem/pem_all.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pem/pem_err.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pem/pem_info.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pem/pem_lib.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pem/pem_oth.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pem/pem_pk8.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pem/pem_pkey.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pem/pem_seal.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pem/pem_sign.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pem/pem_x509.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pem/pem_xaux.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pem/pvkfmt.c

#Name="dsa"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dsa/dsa_ameth.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dsa/dsa_asn1.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dsa/dsa_depr.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dsa/dsa_err.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dsa/dsa_gen.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dsa/dsa_key.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dsa/dsa_lib.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dsa/dsa_ossl.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dsa/dsa_pmeth.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dsa/dsa_prn.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dsa/dsa_sign.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dsa/dsa_vrf.c

#Name="dh"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dh/dh_ameth.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dh/dh_asn1.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dh/dh_check.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dh/dh_depr.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dh/dh_err.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dh/dh_gen.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dh/dh_key.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dh/dh_lib.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dh/dh_pmeth.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dh/dh_prn.c

#Name="bf"

#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bf/bf_cfb64.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bf/bf_ecb.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bf/bf_enc.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bf/bf_ofb64.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/bf/bf_skey.c

#Name="cast"

#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/cast/c_cfb64.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/cast/c_ecb.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/cast/c_enc.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/cast/c_ofb64.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/cast/c_skey.c

#Name="mdc2"

#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/mdc2/mdc2_one.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/mdc2/mdc2dgst.c

#Name="ripemd"

#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ripemd/rmd_dgst.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ripemd/rmd_one.c

#Name="whirlpool"

#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/whrlpool/wp_block.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/whrlpool/wp_dgst.c

#Name="modes"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/modes/cbc128.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/modes/cfb128.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/modes/ctr128.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/modes/cts128.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/modes/ofb128.c

#Name="ecdh"

#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ecdh/ech_err.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ecdh/ech_key.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ecdh/ech_lib.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ecdh/ech_ossl.c

#Name="pkcs7"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pkcs7/bio_pk7.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pkcs7/pk7_asn1.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pkcs7/pk7_attr.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pkcs7/pk7_dgst.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pkcs7/pk7_doit.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pkcs7/pk7_lib.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pkcs7/pk7_mime.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pkcs7/pk7_smime.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pkcs7/pkcs7err.c

#Name="pkcs12"

LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pkcs12/p12_add.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pkcs12/p12_asn.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pkcs12/p12_attr.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pkcs12/p12_crpt.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pkcs12/p12_crt.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pkcs12/p12_decr.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pkcs12/p12_init.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pkcs12/p12_key.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pkcs12/p12_kiss.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pkcs12/p12_mutl.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pkcs12/p12_npas.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pkcs12/p12_p8d.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pkcs12/p12_p8e.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pkcs12/p12_utl.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/pkcs12/pk12err.c

#Name="dso"
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dso/dso_beos.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dso/dso_dl.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dso/dso_dlfcn.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dso/dso_err.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dso/dso_lib.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dso/dso_null.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dso/dso_openssl.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dso/dso_vms.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/dso/dso_win32.c

#Name="cms"
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/cms/cms_asn1.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/cms/cms_att.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/cms/cms_cd.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/cms/cms_dd.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/cms/cms_enc.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/cms/cms_env.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/cms/cms_err.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/cms/cms_ess.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/cms/cms_io.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/cms/cms_lib.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/cms/cms_sd.c
#LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/cms/cms_smime.c

#Name="ts"
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ts/ts_asn1.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ts/ts_conf.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ts/ts_err.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ts/ts_lib.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ts/ts_req_print.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ts/ts_req_utils.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ts/ts_rsp_print.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ts/ts_rsp_sign.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ts/ts_rsp_utils.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ts/ts_rsp_verify.c
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/ts/ts_verify_ctx.c

#Name="txt_db"
LOCAL_SRC_FILES += $(SRC_PATH)/3rdparty/openssl-1.0.0d/crypto/txt_db/txt_db.c

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

