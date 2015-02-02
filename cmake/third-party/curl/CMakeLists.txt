cmake_minimum_required ( VERSION 2.8.5 )

project ( curl )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCURL_STATICLIB -DBUILDING_LIBCURL -DCURL_DISABLE_LDAP" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCURL_STATICLIB -DBUILDING_LIBCURL -DCURL_DISABLE_LDAP" )
  
  if ( BUILD_ANDROID OR BUILD_IOS )
    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_ARES" )
    set ( USE_ARES true )
  endif (BUILD_ANDROID OR BUILD_IOS)

  if ( MOAI_OPENSSL )
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -DUSE_SSLEAY -DUSE_OPENSSL ")
  endif ( MOAI_OPENSSL )


  if (BUILD_LINUX)
      set ( CURL_INCLUDES 
        "${MOAI_ROOT}/3rdparty/curl-7.19.7/include/" 
       )
      set ( RUN_CURL_CONFIGURE true )
  endif (BUILD_LINUX)


  if (BUILD_WINDOWS)
      set ( CURL_INCLUDES 
        "${MOAI_ROOT}/3rdparty/curl-7.19.7/include/" 
     
      )
  endif (BUILD_WINDOWS)
  
  if (BUILD_OSX)
        set ( CURL_INCLUDES
          "${MOAI_ROOT}/3rdparty/curl-7.19.7/include-osx/"
        )
    endif (BUILD_OSX)

  if (BUILD_HTML)
        set ( CURL_INCLUDES
          "${MOAI_ROOT}/3rdparty/curl-7.19.7/include/"
        )
    endif (BUILD_HTML)


  if (BUILD_ANDROID)
    set ( CURL_INCLUDES 
        "${MOAI_ROOT}/3rdparty/curl-7.19.7/include-android/" 
       )
  endif (BUILD_ANDROID)

  if (BUILD_IOS)
    set ( CURL_INCLUDES 
        "${MOAI_ROOT}/3rdparty/curl-7.19.7/include-iphone/"
       )
  endif (BUILD_IOS)

  set (CURL_HEADERS "")
  if (RUN_CURL_CONFIGURE)

      
      set (SSL_PARAMS "--without-ssl")

      if (MOAI_OPENSSL)
         get_target_property(SSLDIR ssl ARCHIVE_OUTPUT_DIRECTORY )
         
         set(SSL_PARAMS "--with-ssl=$<TARGET_FILE_DIR:ssl> --with-ca-path=/etc/ssl/certs/")
      endif (MOAI_OPENSSL)

      add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/config.status"
                       COMMAND "/bin/sh"
                       "${MOAI_ROOT}/3rdparty/curl-7.19.7/configure"
                       "--no-create" "--disable-shared" "--disable-optimize" ${SSL_PARAMS} "--disable-ldap"
                       "--without-libidn" "--without-libssh2" 
                       "CC=${CMAKE_C_COMPILER}"
                       "CFLAGS=${CMAKE_C_FLAGS} -DZL_REPLACE_H"
                       DEPENDS ssl
                       WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
                      )
      

      add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/include/curl/curlbuild.h"
                                "${CMAKE_CURRENT_BINARY_DIR}/include/curl/curl_config.h"
                        COMMAND "${CMAKE_CURRENT_BINARY_DIR}/config.status"
                       "--header=include/curl/curlbuild.h"
                       "--header=include/curl/curl_config.h:lib/curl_config.h.in"
                       DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/config.status"
                       WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

    

    set ( CURL_INCLUDES
          "${CMAKE_CURRENT_BINARY_DIR}/include/curl"
          "${CMAKE_CURRENT_BINARY_DIR}/include" ${CURL_INCLUDES} )

    set ( CURL_HEADERS "${CMAKE_CURRENT_BINARY_DIR}/include/curl/curl_config.h"
                       "${CMAKE_CURRENT_BINARY_DIR}/include/curl/curlbuild.h")
  endif (RUN_CURL_CONFIGURE)



  set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/curl-7.19.7/lib" )
  set ( CURL_SOURCES 
   
    #${CMAKE_CURRENT_SOURCE_DIR}/amigaos.c
    ${CMAKE_CURRENT_SOURCE_DIR}/base64.c
    ${CMAKE_CURRENT_SOURCE_DIR}/connect.c
    ${CMAKE_CURRENT_SOURCE_DIR}/content_encoding.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cookie.c
    ${CMAKE_CURRENT_SOURCE_DIR}/curl_addrinfo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/curl_memrchr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/curl_rand.c
    ${CMAKE_CURRENT_SOURCE_DIR}/curl_sspi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/dict.c
    ${CMAKE_CURRENT_SOURCE_DIR}/easy.c
    ${CMAKE_CURRENT_SOURCE_DIR}/escape.c
    ${CMAKE_CURRENT_SOURCE_DIR}/file.c
    ${CMAKE_CURRENT_SOURCE_DIR}/formdata.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ftp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/getenv.c
    ${CMAKE_CURRENT_SOURCE_DIR}/getinfo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/gtls.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hostares.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hostasyn.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hostip.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hostip4.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hostip6.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hostsyn.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hostthre.c
    ${CMAKE_CURRENT_SOURCE_DIR}/http.c
    ${CMAKE_CURRENT_SOURCE_DIR}/http_chunks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/http_digest.c
    ${CMAKE_CURRENT_SOURCE_DIR}/http_negotiate.c
    ${CMAKE_CURRENT_SOURCE_DIR}/http_ntlm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/if2ip.c
    ${CMAKE_CURRENT_SOURCE_DIR}/inet_ntop.c
    ${CMAKE_CURRENT_SOURCE_DIR}/inet_pton.c
    ${CMAKE_CURRENT_SOURCE_DIR}/krb4.c
    ${CMAKE_CURRENT_SOURCE_DIR}/krb5.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ldap.c
    ${CMAKE_CURRENT_SOURCE_DIR}/llist.c
    ${CMAKE_CURRENT_SOURCE_DIR}/md5.c
    ${CMAKE_CURRENT_SOURCE_DIR}/memdebug.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mprintf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/multi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/netrc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/nonblock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/nss.c
    ${CMAKE_CURRENT_SOURCE_DIR}/nwlib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/nwos.c
    ${CMAKE_CURRENT_SOURCE_DIR}/parsedate.c
    ${CMAKE_CURRENT_SOURCE_DIR}/progress.c
    ${CMAKE_CURRENT_SOURCE_DIR}/qssl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/rawstr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/security.c
    ${CMAKE_CURRENT_SOURCE_DIR}/select.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sendf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/share.c
    ${CMAKE_CURRENT_SOURCE_DIR}/slist.c
    ${CMAKE_CURRENT_SOURCE_DIR}/socks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/socks_gssapi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/socks_sspi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/speedcheck.c
    ${CMAKE_CURRENT_SOURCE_DIR}/splay.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ssh.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sslgen.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ssluse.c
    ${CMAKE_CURRENT_SOURCE_DIR}/strdup.c
    ${CMAKE_CURRENT_SOURCE_DIR}/strequal.c
    ${CMAKE_CURRENT_SOURCE_DIR}/strerror.c
    ${CMAKE_CURRENT_SOURCE_DIR}/strtok.c
    ${CMAKE_CURRENT_SOURCE_DIR}/strtoofft.c
    ${CMAKE_CURRENT_SOURCE_DIR}/telnet.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tftp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/timeval.c
    ${CMAKE_CURRENT_SOURCE_DIR}/transfer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/url.c
    ${CMAKE_CURRENT_SOURCE_DIR}/version.c

   )


 
  add_library(curl STATIC ${CURL_SOURCES} ${CURL_HEADERS})
  target_include_directories (curl PUBLIC "$<BUILD_INTERFACE:${CURL_INCLUDES}>" PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
  target_link_libraries( curl zlib )
  set_target_properties( curl PROPERTIES FOLDER Third-Party )

  if (USE_ARES)  
    target_link_libraries( curl cares )
  endif (USE_ARES)

  if (MOAI_OPENSSL)
     target_link_libraries( curl ssl )
  endif (MOAI_OPENSSL)
 
  if (BUILD_WINDOWS)
    target_link_libraries( curl ws2_32 )
  endif (BUILD_WINDOWS)

  install(TARGETS curl EXPORT libmoai ARCHIVE DESTINATION lib)