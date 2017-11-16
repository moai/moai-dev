cmake_minimum_required ( VERSION 2.8.11 )
project ( cares )

set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCARES_STATICLIB" )
set ( CARES_INCLUDES "${MOAI_ROOT}/3rdparty/c-ares-1.7.5/" )

if (BUILD_ANDROID)
    set ( CARES_INCLUDES 
        ${CARES_INCLUDES}
        "${MOAI_ROOT}/3rdparty/c-ares-1.7.5/include-android/"
       )
  endif (BUILD_ANDROID)

if (BUILD_IOS)
    set ( CARES_INCLUDES 
       ${CARES_INCLUDES}
        "${MOAI_ROOT}/3rdparty/c-ares-1.7.5/include-apple/" 
       )
  endif (BUILD_IOS)

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
 
    )

add_library( cares STATIC ${CARES_SRC})
target_include_directories ( cares PUBLIC "$<BUILD_INTERFACE:${CARES_INCLUDES}>" )
set_target_properties( cares PROPERTIES FOLDER Third-Party )
install(TARGETS cares EXPORT libmoai ARCHIVE DESTINATION lib)