cmake_minimum_required ( VERSION 2.8.5 )

project ( curl )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DBUILDING_LIBCURL -DCURL_DISABLE_LDAP -DCURL_DISABLE_NTLM -DUSE_MBEDTLS"  )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILDING_LIBCURL -DCURL_DISABLE_LDAP -DCURL_DISABLE_NTLM -DUSE_MBEDTLS" )
  
  if ( BUILD_ANDROID OR BUILD_IOS )
    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_ARES" )
    set ( USE_ARES true )
  endif (BUILD_ANDROID OR BUILD_IOS)

  if (BUILD_LINUX)
      set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -DHAVE_CONFIG_H"  )
      set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_CONFIG_H" )
      set ( CURL_INCLUDES 
        "${MOAI_ROOT}/src/custom-include/curl/linux"
        "${MOAI_ROOT}/src/custom-include/curl/linux/curl"
        "${MOAI_ROOT}/3rdparty/curl/include/" 
        "${MOAI_ROOT}/3rdparty/curl/lib/" 
       )
      set ( RUN_CURL_CONFIGURE true )
  endif (BUILD_LINUX)


  if (BUILD_WINDOWS)
      set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DWIN32 -D_WINDOWS"  )
      set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWIN32 -D_WINDOWS" )
      set ( CURL_INCLUDES 
      "${MOAI_ROOT}/src/custom-include/curl/windows"
      "${MOAI_ROOT}/src/custom-include/curl/windows/curl"
      "${MOAI_ROOT}/3rdparty/curl/include/" 
      "${MOAI_ROOT}/3rdparty/curl/lib/" 
     
      )
  endif (BUILD_WINDOWS)
  
  if (BUILD_OSX)
        set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_CONFIG_H"  )
        set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_CONFIG_H" )
        set ( CURL_INCLUDES
          "${MOAI_ROOT}/src/custom-include/curl/osx" 
          "${MOAI_ROOT}/src/custom-include/curl/osx/curl" 
          "${MOAI_ROOT}/3rdparty/curl/include/"
          "${MOAI_ROOT}/3rdparty/curl/lib"
        )
    endif (BUILD_OSX)

  if (BUILD_HTML)
        set ( CURL_INCLUDES
          "${MOAI_ROOT}/3rdparty/curl/include/"
        )
    endif (BUILD_HTML)


  if (BUILD_ANDROID)
    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_CONFIG_H"  )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_CONFIG_H" )
    set ( CURL_INCLUDES 
        "${MOAI_ROOT}/src/custom-include/curl/android" 
        "${MOAI_ROOT}/src/custom-include/curl/android/curl" 
        "${MOAI_ROOT}/3rdparty/curl/include/"
        "${MOAI_ROOT}/3rdparty/curl/lib"
       )
  endif (BUILD_ANDROID)

  if (BUILD_IOS)
    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_CONFIG_H"  )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_CONFIG_H" )
    set ( CURL_INCLUDES 
        "${MOAI_ROOT}/src/custom-include/curl/ios" 
        "${MOAI_ROOT}/src/custom-include/curl/ios/curl" 
        "${MOAI_ROOT}/3rdparty/curl/include/"
        "${MOAI_ROOT}/3rdparty/curl/lib"
    )
  endif (BUILD_IOS)

  set (CURL_HEADERS "")

  set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/curl" )
  set ( CURL_SOURCES 
   
   # ${CMAKE_CURRENT_SOURCE_DIR}/lib/amigaos.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/asyn-ares.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/asyn-thread.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/base64.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/conncache.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/connect.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/content_encoding.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/cookie.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/curl_addrinfo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/curl_des.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/curl_endian.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/curl_fnmatch.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/curl_gethostname.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/curl_gssapi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/curl_memrchr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/curl_multibyte.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/curl_ntlm_core.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/curl_ntlm_wb.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/curl_rtmp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/curl_sasl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/curl_sspi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/curl_threads.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/dict.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/dotdot.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/easy.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/escape.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/file.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/fileinfo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/formdata.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ftp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ftplistparser.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/getenv.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/getinfo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/gopher.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/hash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/hmac.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/hostasyn.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/hostcheck.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/hostip.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/hostip4.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/hostip6.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/hostsyn.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/http.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/http2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/http_chunks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/http_digest.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/http_negotiate.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/http_ntlm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/http_proxy.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/idn_win32.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/if2ip.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/imap.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/inet_ntop.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/inet_pton.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/krb5.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ldap.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/llist.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/md4.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/md5.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/memdebug.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/mprintf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/multi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/netrc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/non-ascii.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/nonblock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/nwlib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/nwos.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/openldap.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/parsedate.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/pingpong.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/pipeline.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/pop3.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/progress.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/rand.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/rtsp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/security.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/select.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/sendf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/share.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/slist.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/smb.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/smtp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/socks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/socks_gssapi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/socks_sspi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/speedcheck.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/splay.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ssh.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/strcase.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/strdup.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/strerror.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/strtok.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/strtoofft.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/system_win32.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/telnet.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/tftp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/timeval.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/transfer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/url.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/version.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/warnless.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/wildcard.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/x509asn1.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/vauth/cleartext.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/vauth/cram.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/vauth/digest.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/vauth/digest_sspi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/vauth/krb5_gssapi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/vauth/krb5_sspi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/vauth/ntlm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/vauth/ntlm_sspi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/vauth/oauth2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/vauth/spnego_gssapi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/vauth/spnego_sspi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/vauth/vauth.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/vtls/axtls.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/vtls/cyassl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/vtls/darwinssl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/vtls/gskit.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/vtls/gtls.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/vtls/mbedtls.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/vtls/nss.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/vtls/openssl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/vtls/polarssl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/vtls/polarssl_threadlock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/vtls/schannel.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/vtls/vtls.c


   )


 
  add_library(curl STATIC ${CURL_SOURCES} ${CURL_HEADERS})
  target_include_directories (curl PUBLIC "$<BUILD_INTERFACE:${CURL_INCLUDES}>" )
  target_link_libraries( curl zlib mbedtls)
  set_target_properties( curl PROPERTIES FOLDER Third-Party )
  target_compile_definitions( curl PUBLIC CURL_STATICLIB=1 )

  if (USE_ARES)  
    target_link_libraries( curl cares )
  endif (USE_ARES)

  if (BUILD_WINDOWS)
    target_link_libraries( curl ws2_32 )
  endif (BUILD_WINDOWS)

  install(TARGETS curl EXPORT libmoai ARCHIVE DESTINATION lib)