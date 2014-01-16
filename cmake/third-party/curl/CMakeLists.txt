cmake_minimum_required ( VERSION 2.8.5 )

project ( curl )

if ( MOAI_CURL )

  


  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_LIBCURL=1 -DCURL_STATICLIB -DBUILDING_LIBCURL -DCURL_DISABLE_LDAP" PARENT_SCOPE)
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_LIBCURL=1 -DCURL_STATICLIB -DBUILDING_LIBCURL -DCURL_DISABLE_LDAP" PARENT_SCOPE)

  
  if (BUILD_ANDROID)
    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_SSL -DUSE_SSLEAY -DUSE_OPENSSL  -DUSE_ARES -DCARES_STATICLIB" PARENT_SCOPE)
  endif (BUILD_ANDROID)

  set ( CARES_SRC "" )
  if (BUILD_LINUX)
      set ( CURL_INCLUDES 
        "${MOAI_ROOT}/3rdparty/curl-7.19.7/include/" 
        "${CMAKE_CURRENT_SOURCE_DIR}/curl/"
		"${CMAKE_CURRENT_SOURCE_DIR}"
      PARENT_SCOPE )
  endif (BUILD_LINUX)


  if (BUILD_WINDOWS)
      set ( CURL_INCLUDES 
        "${MOAI_ROOT}/3rdparty/curl-7.19.7/include/" 
        "${CMAKE_CURRENT_SOURCE_DIR}/curl/"
     PARENT_SCOPE )
  endif (BUILD_WINDOWS)
  
  if (BUILD_OSX)
        set ( CURL_INCLUDES
          "${MOAI_ROOT}/3rdparty/curl-7.19.7/include-osx/"
        PARENT_SCOPE )
    endif (BUILD_OSX)


  if (BUILD_ANDROID)
    set ( CURL_INCLUDES 
        "${MOAI_ROOT}/3rdparty/curl-7.19.7/include-android/" 
        "${MOAI_ROOT}/3rdparty/c-ares-1.7.5/"
        "${MOAI_ROOT}/3rdparty/c-ares-1.7.5/include-android/"
      PARENT_SCOPE )
  endif (BUILD_ANDROID)

if (BUILD_IOS)
    set ( CURL_INCLUDES 
        "${MOAI_ROOT}/3rdparty/curl-7.19.7/include-iphone/"
        "${MOAI_ROOT}/3rdparty/c-ares-1.7.5/"
        "${MOAI_ROOT}/3rdparty/c-ares-1.7.5/include-apple/" 
      PARENT_SCOPE )
  endif (BUILD_IOS)

  if (BUILD_ANDROID OR BUILD_IOS)  
    set (CARES_SRC 
         ${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares__close_sockets.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares__get_hostent.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares__read_line.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares__timeval.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_cancel.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_data.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_destroy.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_expand_name.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_expand_string.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_fds.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_free_hostent.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_free_string.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_getenv.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_gethostbyaddr.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_gethostbyname.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_getnameinfo.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_getopt.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_getsock.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_init.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_library_init.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_llist.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_mkquery.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_nowarn.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_options.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_parse_a_reply.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_parse_aaaa_reply.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_parse_mx_reply.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_parse_ns_reply.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_parse_ptr_reply.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_parse_srv_reply.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_parse_txt_reply.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_platform.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_process.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_query.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_search.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_send.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_strcasecmp.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_strdup.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_strerror.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_timeout.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_version.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_writev.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/bitncmp.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/inet_net_pton.c
	${MOAI_ROOT}/3rdparty/c-ares-1.7.5/ares_inet_ntop.c
    PARENT_SCOPE
    )
    
  endif (BUILD_ANDROID OR BUILD_IOS)

       set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/curl-7.19.7/lib" )
  set ( CURL_SOURCES 
    ${CARES_SRC}
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

  PARENT_SCOPE )

else ( MOAI_CURL )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_LIBCURL=0" PARENT_SCOPE)
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_LIBCURL=0" PARENT_SCOPE)

endif ( MOAI_CURL )
