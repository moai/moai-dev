cmake_minimum_required(VERSION 2.8.5)
project(cares)

# Check to ensure that we are running as part of the
# root CMakeLists.txt file.
if(NOT DEFINED IS_ROOT)
  message(FATAL_ERROR "You must generate the CMake files as part of the root directory.")
endif(NOT DEFINED IS_ROOT)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(${PROJECT_NAME}_UnixInclude "${ROOT_DIR}/3rdparty/c-ares-1.7.5/" PARENT_SCOPE)
add_definitions(-DHAVE_CONFIG_H
                -Dh_addr=h_addr_list[0])
include_directories("${ROOT_DIR}/cmake/libcares/")
include_directories("${ROOT_DIR}/cmake/libcurl/")
include_directories("${ROOT_DIR}/3rdparty/c-ares-1.7.5/")
include_directories("${ROOT_DIR}/3rdparty/curl-7.19.7/include/")
set(CMAKE_CURRENT_SOURCE_DIR "${ROOT_DIR}/3rdparty/c-ares-1.7.5/")

moai_manual(library
    ${CMAKE_CURRENT_SOURCE_DIR}/ares__close_sockets.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares__get_hostent.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares__read_line.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares__timeval.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_cancel.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_data.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_destroy.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_expand_name.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_expand_string.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_fds.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_free_hostent.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_free_string.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_getenv.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_gethostbyaddr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_gethostbyname.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_getnameinfo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_getopt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_getsock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_inet_ntop.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_init.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_library_init.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_llist.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_mkquery.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_nowarn.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_options.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_parse_a_reply.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_parse_aaaa_reply.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_parse_mx_reply.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_parse_ns_reply.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_parse_ptr_reply.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_parse_srv_reply.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_parse_txt_reply.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_platform.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_process.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_query.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_search.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_send.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_strcasecmp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_strdup.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_strerror.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_timeout.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_version.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ares_writev.c
    ${CMAKE_CURRENT_SOURCE_DIR}/bitncmp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/inet_net_pton.c
)
moai_needs(curl)