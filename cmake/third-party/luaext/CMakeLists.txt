cmake_minimum_required ( VERSION 2.8.5 )
project ( luaext )

if ( MOAI_LUAEXT )

  set ( CMAKE_CURRENT_SOURCE_DIR "../../../3rdparty/" )
  
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_LUAEXT=1" CACHE INTERNAL "doc string" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_LUAEXT=1" CACHE INTERNAL "doc string" )

  set ( LUAEXT_INCLUDES "../../3rdparty/lua-5.1.3/src/;../../3rdparty/luasocket-2.0.2/src/" CACHE INTERNAL "doc string" )

  set ( LUAEXT_SOURCES 
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
    # ${CMAKE_CURRENT_SOURCE_DIR}/luasocket-2.0.2/src/wsocket.c
    ${CMAKE_CURRENT_SOURCE_DIR}/luacrypto-0.2.0/src/lcrypto.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/luacurl-1.2.1/luacurl.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/luasql-2.2.0/src/ls_sqlite3.c
    ${CMAKE_CURRENT_SOURCE_DIR}/luasql-2.2.0/src/luasql.c
    ${CMAKE_CURRENT_SOURCE_DIR}/luafilesystem-1.5.0/src/lfs.c
    # Manually added:
    ${CMAKE_CURRENT_SOURCE_DIR}/luasocket-2.0.2/src/usocket.c
  
  CACHE INTERNAL "doc string" )

else ( MOAI_LUAEXT )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_LUAEXT=0" CACHE INTERNAL "doc string" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_LUAEXT=0" CACHE INTERNAL "doc string" )

endif ( MOAI_LUAEXT )
