cmake_minimum_required(VERSION 2.8.5)
project(ssl)

# Check to ensure that we are running as part of the
# root CMakeLists.txt file.
if(NOT DEFINED IS_ROOT)
  message(FATAL_ERROR "You must generate the CMake files as part of the root directory.")
endif(NOT DEFINED IS_ROOT)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
set(${PROJECT_NAME}_UnixInclude "${ROOT_DIR}/3rdparty/openssl-1.0.0d/include/" PARENT_SCOPE)
add_definitions(${CRYPTO_DEFINITIONS})
include_directories("${ROOT_DIR}/cmake/libcrypto/")
include_directories("${ROOT_DIR}/3rdparty/openssl-1.0.0d/crypto/")
include_directories("${ROOT_DIR}/3rdparty/openssl-1.0.0d/include/")
include_directories("${ROOT_DIR}/3rdparty/openssl-1.0.0d/")
set(CMAKE_CURRENT_SOURCE_DIR "${ROOT_DIR}/3rdparty/openssl-1.0.0d/ssl/")

moai_manual(library
    ${CMAKE_CURRENT_SOURCE_DIR}/bio_ssl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/d1_both.c
    ${CMAKE_CURRENT_SOURCE_DIR}/d1_clnt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/d1_enc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/d1_lib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/d1_meth.c
    ${CMAKE_CURRENT_SOURCE_DIR}/d1_pkt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/d1_srvr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/kssl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/s23_clnt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/s23_lib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/s23_meth.c
    ${CMAKE_CURRENT_SOURCE_DIR}/s23_pkt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/s23_srvr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/s2_clnt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/s2_enc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/s2_lib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/s2_meth.c
    ${CMAKE_CURRENT_SOURCE_DIR}/s2_pkt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/s2_srvr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/s3_both.c
    ${CMAKE_CURRENT_SOURCE_DIR}/s3_clnt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/s3_enc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/s3_lib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/s3_meth.c
    ${CMAKE_CURRENT_SOURCE_DIR}/s3_pkt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/s3_srvr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ssl_algs.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ssl_asn1.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ssl_cert.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ssl_ciph.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ssl_err2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ssl_err.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ssl_lib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ssl_rsa.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ssl_sess.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ssl_stat.c
    #${CMAKE_CURRENT_SOURCE_DIR}/ssl_task.c
    #${CMAKE_CURRENT_SOURCE_DIR}/ssltest.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ssl_txt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/t1_clnt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/t1_enc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/t1_lib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/t1_meth.c
    ${CMAKE_CURRENT_SOURCE_DIR}/t1_reneg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/t1_srvr.c
)
moai_needs(crypto)