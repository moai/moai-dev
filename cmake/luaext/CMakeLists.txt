cmake_minimum_required(VERSION 2.8.5)
project(luaext)

# Check to ensure that we are running as part of the
# root CMakeLists.txt file.
if(NOT DEFINED IS_ROOT)
  message(FATAL_ERROR "You must generate the CMake files as part of the root directory.")
endif(NOT DEFINED IS_ROOT)

include_directories("${ROOT_DIR}/3rdparty/lua-5.1.3/src/")
include_directories("${ROOT_DIR}/3rdparty/luasocket-2.0.2/src/")
set(CMAKE_CURRENT_SOURCE_DIR "${ROOT_DIR}/3rdparty/")

moai_manual(library
    # Generated with:
    #    grep "ClCompile " ../../vs2010/luaext/luaext.vcxproj \
    #       | awk '//{printf "%s%s\n", "${CMAKE_CURRENT_SOURCE_DIR}/", substr($2,25,length($2)-25)}' \
    #       | sed 's/\\/\//g'
    ${CMAKE_CURRENT_SOURCE_DIR}/luasocket-2.0.2-embed/fullluasocket.c
    ${CMAKE_CURRENT_SOURCE_DIR}/luasocket-2.0.2-embed/luasocketscripts.c
    ${CMAKE_CURRENT_SOURCE_DIR}/luasocket-2.0.2/src/auxiliar.c
    ${CMAKE_CURRENT_SOURCE_DIR}/luasocket-2.0.2/src/buffer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/luasocket-2.0.2/src/except.c
    ${CMAKE_CURRENT_SOURCE_DIR}/luasocket-2.0.2/src/inet.c
    ${CMAKE_CURRENT_SOURCE_DIR}/luasocket-2.0.2/src/io.c
    ${CMAKE_CURRENT_SOURCE_DIR}/luasocket-2.0.2/src/luasocket.c
    ${CMAKE_CURRENT_SOURCE_DIR}/luasocket-2.0.2/src/mime.c
    ${CMAKE_CURRENT_SOURCE_DIR}/luasocket-2.0.2/src/options.c
    ${CMAKE_CURRENT_SOURCE_DIR}/luasocket-2.0.2/src/select.c
    ${CMAKE_CURRENT_SOURCE_DIR}/luasocket-2.0.2/src/tcp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/luasocket-2.0.2/src/timeout.c
    ${CMAKE_CURRENT_SOURCE_DIR}/luasocket-2.0.2/src/udp.c
    #${CMAKE_CURRENT_SOURCE_DIR}/luasocket-2.0.2/src/wsocket.c
    ${CMAKE_CURRENT_SOURCE_DIR}/luacrypto-0.2.0/src/lcrypto.c
    ${CMAKE_CURRENT_SOURCE_DIR}/luacurl-1.2.1/luacurl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/luasql-2.2.0/src/ls_sqlite3.c
    ${CMAKE_CURRENT_SOURCE_DIR}/luasql-2.2.0/src/luasql.c
    ${CMAKE_CURRENT_SOURCE_DIR}/luafilesystem-1.5.0/src/lfs.c
    # Manually added:
    ${CMAKE_CURRENT_SOURCE_DIR}/luasocket-2.0.2/src/usocket.c
)
moai_needs(crypto)
moai_needs(curl)
