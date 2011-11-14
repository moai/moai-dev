#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	#Name="rand"
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rand/md_rand.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rand/rand_egd.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rand/rand_err.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rand/rand_lib.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rand/rand_nw.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rand/rand_os2.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rand/rand_unix.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rand/rand_win.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rand/randfile.c

	#Name="rsa"
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_ameth.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_asn1.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_chk.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_depr.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_eay.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_err.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_gen.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_lib.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_none.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_null.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_oaep.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_pk1.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_pmeth.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_prn.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_pss.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_saos.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_sign.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_ssl.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/rsa/rsa_x931.c

		#Name="dso"
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/dso/dso_beos.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/dso/dso_dl.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/dso/dso_dlfcn.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/dso/dso_err.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/dso/dso_lib.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/dso/dso_null.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/dso/dso_openssl.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/dso/dso_vms.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/dso/dso_win32.c

	#Name="cms"
	#LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/cms/cms_asn1.c
	#LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/cms/cms_att.c
	#LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/cms/cms_cd.c
	#LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/cms/cms_dd.c
	#LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/cms/cms_enc.c
	#LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/cms/cms_env.c
	#LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/cms/cms_err.c
	#LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/cms/cms_ess.c
	#LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/cms/cms_io.c
	#LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/cms/cms_lib.c
	#LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/cms/cms_sd.c
	#LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/cms/cms_smime.c

	#Name="ts"
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/ts/ts_asn1.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/ts/ts_conf.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/ts/ts_err.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/ts/ts_lib.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/ts/ts_req_print.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/ts/ts_req_utils.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/ts/ts_rsp_print.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/ts/ts_rsp_sign.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/ts/ts_rsp_utils.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/ts/ts_rsp_verify.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/ts/ts_verify_ctx.c

	#Name="txt_db"
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/txt_db/txt_db.c

	#Name="x509"
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509/by_dir.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509/by_file.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509/x509_att.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509/x509_cmp.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509/x509_d2.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509/x509_def.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509/x509_err.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509/x509_ext.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509/x509_lu.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509/x509_obj.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509/x509_r2x.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509/x509_req.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509/x509_set.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509/x509_trs.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509/x509_txt.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509/x509_v3.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509/x509_vfy.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509/x509_vpm.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509/x509cset.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509/x509name.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509/x509rset.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509/x509spki.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509/x509type.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509/x_all.c

	#Name="x509v3"
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/pcy_cache.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/pcy_data.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/pcy_lib.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/pcy_map.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/pcy_node.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/pcy_tree.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_addr.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_akey.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_akeya.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_alt.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_asid.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_bcons.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_bitst.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_conf.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_cpols.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_crld.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_enum.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_extku.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_genn.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_ia5.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_info.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_int.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_lib.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_ncons.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_ocsp.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_pci.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_pcia.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_pcons.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_pku.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_pmaps.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_prn.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_purp.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_skey.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_sxnet.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3_utl.c
	LOCAL_SRC_FILES += $(MY_MOAI_ROOT)/3rdparty/openssl-1.0.0d/crypto/x509v3/v3err.c
	