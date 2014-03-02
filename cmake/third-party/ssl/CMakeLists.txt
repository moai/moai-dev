cmake_minimum_required ( VERSION 2.8.5 )
project ( ssl )


 set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/openssl-1.0.0d/" )
 
 set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DL_ENDIAN -DOPENSSL_NO_ASM  -DOPENSSL_NO_RC5 -DOPENSSL_NO_MD2 -DOPENSSL_NO_KRB5 -DOPENSSL_NO_JPAKE -DOPENSSL_NO_STATIC_ENGINE "  )


 if ( BUILD_LINUX )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DOPENSSL_SYSNAME_LINUX"  )
  set( PLATFORM_INCLUDE "${MOAI_ROOT}/3rdparty/openssl-1.0.0d/include-linux" "${MOAI_ROOT}/3rdparty/openssl-1.0.0d/include/"  )
 endif ( BUILD_LINUX)

 if ( BUILD_WINDOWS )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DOPENSSL_SYSNAME_WIN32 "  )
  if (MSVC)
    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DWIN32 -DWIN32_LEAN_AND_MEAN -D_LIB -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE")
    set ( PLATFORM_INCLUDE "${MOAI_ROOT}/3rdparty/openssl-1.0.0d/include-win32" )
  else()
    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMK1MF_BUILD")
    set ( PLATFORM_INCLUDE "${MOAI_ROOT}/3rdparty/openssl-1.0.0d/include-mingw" )
  endif (MSVC)
 endif ( BUILD_WINDOWS )


 if ( BUILD_ANDROID )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DOPENSSL_SYSNAME_ANDROID " )
  set( PLATFORM_INCLUDE "${MOAI_ROOT}/3rdparty/openssl-1.0.0d/include-android" )
 endif ( BUILD_ANDROID )

 if ( BUILD_IOS )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DOPENSSL_SYSNAME_MACOSX "  )
  set( PLATFORM_INCLUDE "${MOAI_ROOT}/3rdparty/openssl-1.0.0d/include-iphone" )
 endif ( BUILD_IOS )

  if ( BUILD_OSX )
   set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DOPENSSL_SYSNAME_MACOSX "  )
   set( PLATFORM_INCLUDE "${MOAI_ROOT}/3rdparty/openssl-1.0.0d/include/" )
  endif ( BUILD_OSX )




 set ( SSL_INCLUDES 
    ${PLATFORM_INCLUDE}
    "${MOAI_ROOT}/3rdparty/openssl-1.0.0d/crypto/"
    "${MOAI_ROOT}/3rdparty/openssl-1.0.0d/crypto/asn1/"
    "${MOAI_ROOT}/3rdparty/openssl-1.0.0d/crypto/evp/"
    "${MOAI_ROOT}/3rdparty/openssl-1.0.0d/"
 )




  set ( SSL_SOURCES 

    ${CMAKE_CURRENT_SOURCE_DIR}ssl/bio_ssl.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/d1_both.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/d1_clnt.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/d1_enc.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/d1_lib.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/d1_meth.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/d1_pkt.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/d1_srvr.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/kssl.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/s23_clnt.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/s23_lib.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/s23_meth.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/s23_pkt.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/s23_srvr.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/s2_clnt.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/s2_enc.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/s2_lib.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/s2_meth.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/s2_pkt.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/s2_srvr.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/s3_both.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/s3_clnt.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/s3_enc.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/s3_lib.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/s3_meth.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/s3_pkt.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/s3_srvr.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/ssl.h
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/ssl_algs.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/ssl_asn1.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/ssl_cert.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/ssl_ciph.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/ssl_err2.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/ssl_err.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/ssl_lib.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/ssl_rsa.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/ssl_sess.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/ssl_stat.c
    #${CMAKE_CURRENT_SOURCE_DIR}ssl/ssl_task.c
    #${CMAKE_CURRENT_SOURCE_DIR}ssl/ssltest.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/ssl_txt.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/t1_clnt.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/t1_enc.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/t1_lib.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/t1_meth.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/t1_reneg.c
    ${CMAKE_CURRENT_SOURCE_DIR}ssl/t1_srvr.c
 )
 
  
add_library (ssl STATIC ${SSL_SOURCES})
target_include_directories (ssl PUBLIC ${SSL_INCLUDES})
set_target_properties( ssl PROPERTIES FOLDER Third-Party )
target_link_libraries( ssl crypto )
