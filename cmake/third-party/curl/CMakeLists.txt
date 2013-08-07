cmake_minimum_required ( VERSION 2.8.5 )

project ( curl )

if ( MOAI_CURL )

  set ( CMAKE_CURRENT_SOURCE_DIR "../../3rdparty/curl-7.19.7/lib" )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_LIBCURL=1" CACHE INTERNAL "doc string")
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_LIBCURL=1" CACHE INTERNAL "doc string")


  set ( CURL_INCLUDES 
    "../../3rdparty/curl-7.19.7/include/" 
    "${CMAKE_BINARY_DIR}/../third-party/curl/"
  CACHE INTERNAL "doc string" )

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

  CACHE INTERNAL "doc string" )

else ( MOAI_CURL )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_LIBCURL=0" CACHE INTERNAL "doc string")
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_LIBCURL=0" CACHE INTERNAL "doc string")

endif ( MOAI_CURL )
