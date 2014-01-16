cmake_minimum_required ( VERSION 2.8.5 )
project ( luaext )

if ( MOAI_LUAEXT )

  set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/" )

  
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_LUAEXT=1" PARENT_SCOPE )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_LUAEXT=1" PARENT_SCOPE )

  set ( LUAEXT_INCLUDES "${MOAI_ROOT}/3rdparty/lua-5.1.3/src/"  PARENT_SCOPE )


  

  set ( LUAEXT_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/luafilesystem-1.5.0/src/lfs.c
  PARENT_SCOPE )

if ( MOAI_SQLITE3 )
	 set ( LUAEXT_SOURCES ${LUAEXT_SOURCES}
      ${CMAKE_CURRENT_SOURCE_DIR}/luasql-2.2.0/src/ls_sqlite3.c
      ${CMAKE_CURRENT_SOURCE_DIR}/luasql-2.2.0/src/luasql.c
      PARENT_SCOPE
    )
endif ( MOAI_SQLITE3 )

if ( MOAI_CURL )
    set ( LUAEXT_SOURCES ${LUAEXT_SOURCES}
      ${CMAKE_CURRENT_SOURCE_DIR}/luacurl-1.2.1/luacurl.c
      PARENT_SCOPE
    )
  endif ( MOAI_CURL )

  if ( MOAI_CRYPTO )
    set ( LUAEXT_SOURCES ${LUAEXT_SOURCES}
      ${CMAKE_CURRENT_SOURCE_DIR}/luacrypto-0.2.0/src/lcrypto.c
      PARENT_SCOPE
    )
  endif ( MOAI_CRYPTO )




set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/" )

 include_directories ( "${CMAKE_CURRENT_SOURCE_DIR}/lua-5.1.3/src/" 
                        "${CMAKE_CURRENT_SOURCE_DIR}/luasocket-2.0.2/src" 
                        "${CMAKE_CURRENT_SOURCE_DIR}/../src/")

  if ( BUILD_WINDOWS )
    set ( LUASOCKET_PLAT  ${CMAKE_CURRENT_SOURCE_DIR}/luasocket-2.0.2/src/wsocket.c )
  else ( BUILD_WINDOWS )
    set ( LUASOCKET_PLAT ${CMAKE_CURRENT_SOURCE_DIR}/luasocket-2.0.2/src/usocket.c )
  endif ( BUILD_WINDOWS )

 
  
  add_library ( luasocket STATIC
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
    ${LUASOCKET_PLAT}

  )
        
   if ( BUILD_WINDOWS )
     target_link_libraries( luasocket Ws2_32 )
   endif (BUILD_WINDOWS)


  

else ( MOAI_LUAEXT )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_LUAEXT=0" PARENT_SCOPE )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_LUAEXT=0" PARENT_SCOPE )

endif ( MOAI_LUAEXT )
