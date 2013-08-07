cmake_minimum_required ( VERSION 2.8.5 )

project ( curl )

if ( MOAI_CURL )

  set ( CMAKE_CURRENT_SOURCE_DIR "../../3rdparty/curl-7.19.7/lib" )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_LIBCURL=1" CACHE INTERNAL "doc string")
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_LIBCURL=1" CACHE INTERNAL "doc string")

  
  if (BUILD_ANDROID)
    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_SSL -DUSE_SSLEAY -DUSE_OPENSSL -DBUILDING_LIBCURL -DUSE_ARES -DCARES_STATICLIB" CACHE INTERNAL "doc string" )
  endif (BUILD_ANDROID)

  set ( CARES_SRC "" )
  if (BUILD_LINUX)
      set ( CURL_INCLUDES 
        "../../3rdparty/curl-7.19.7/include/" 
        "${CMAKE_CURRENT_BINARY_DIR}/curl/"
      CACHE INTERNAL "doc string" )
  endif (BUILD_LINUX)

  if (BUILD_ANDROID)
    set ( CURL_INCLUDES 
        "../../3rdparty/curl-7.19.7/include-android/" 
        "../../3rdparty/c-ares-1.7.5/"
        "../../3rdparty/c-ares-1.7.5/include-android/"
      CACHE INTERNAL "doc string" )
    
    set (CARES_SRC 
      ../../3rdparty/c-ares-1.7.5/ares__close_sockets.c
	../../3rdparty/c-ares-1.7.5/ares__get_hostent.c
	../../3rdparty/c-ares-1.7.5/ares__read_line.c
	../../3rdparty/c-ares-1.7.5/ares__timeval.c
	../../3rdparty/c-ares-1.7.5/ares_cancel.c
	../../3rdparty/c-ares-1.7.5/ares_data.c
	../../3rdparty/c-ares-1.7.5/ares_destroy.c
	../../3rdparty/c-ares-1.7.5/ares_expand_name.c
	../../3rdparty/c-ares-1.7.5/ares_expand_string.c
	../../3rdparty/c-ares-1.7.5/ares_fds.c
	../../3rdparty/c-ares-1.7.5/ares_free_hostent.c
	../../3rdparty/c-ares-1.7.5/ares_free_string.c
	../../3rdparty/c-ares-1.7.5/ares_getenv.c
	../../3rdparty/c-ares-1.7.5/ares_gethostbyaddr.c
	../../3rdparty/c-ares-1.7.5/ares_gethostbyname.c
	../../3rdparty/c-ares-1.7.5/ares_getnameinfo.c
	../../3rdparty/c-ares-1.7.5/ares_getopt.c
	../../3rdparty/c-ares-1.7.5/ares_getsock.c
	../../3rdparty/c-ares-1.7.5/ares_init.c
	../../3rdparty/c-ares-1.7.5/ares_library_init.c
	../../3rdparty/c-ares-1.7.5/ares_llist.c
	../../3rdparty/c-ares-1.7.5/ares_mkquery.c
	../../3rdparty/c-ares-1.7.5/ares_nowarn.c
	../../3rdparty/c-ares-1.7.5/ares_options.c
	../../3rdparty/c-ares-1.7.5/ares_parse_a_reply.c
	../../3rdparty/c-ares-1.7.5/ares_parse_aaaa_reply.c
	../../3rdparty/c-ares-1.7.5/ares_parse_mx_reply.c
	../../3rdparty/c-ares-1.7.5/ares_parse_ns_reply.c
	../../3rdparty/c-ares-1.7.5/ares_parse_ptr_reply.c
	../../3rdparty/c-ares-1.7.5/ares_parse_srv_reply.c
	../../3rdparty/c-ares-1.7.5/ares_parse_txt_reply.c
	../../3rdparty/c-ares-1.7.5/ares_platform.c
	../../3rdparty/c-ares-1.7.5/ares_process.c
	../../3rdparty/c-ares-1.7.5/ares_query.c
	../../3rdparty/c-ares-1.7.5/ares_search.c
	../../3rdparty/c-ares-1.7.5/ares_send.c
	../../3rdparty/c-ares-1.7.5/ares_strcasecmp.c
	../../3rdparty/c-ares-1.7.5/ares_strdup.c
	../../3rdparty/c-ares-1.7.5/ares_strerror.c
	../../3rdparty/c-ares-1.7.5/ares_timeout.c
	../../3rdparty/c-ares-1.7.5/ares_version.c
	../../3rdparty/c-ares-1.7.5/ares_writev.c
	../../3rdparty/c-ares-1.7.5/bitncmp.c
	../../3rdparty/c-ares-1.7.5/inet_net_pton.c
	../../3rdparty/c-ares-1.7.5/ares_inet_ntop.c
    
    )
    
  endif (BUILD_ANDROID)


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

  CACHE INTERNAL "doc string" )

else ( MOAI_CURL )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_LIBCURL=0" CACHE INTERNAL "doc string")
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_LIBCURL=0" CACHE INTERNAL "doc string")

endif ( MOAI_CURL )
