project ( luasocket )

  if ( BUILD_WINDOWS )
    set ( LUASOCKET_PLAT ${MOAI_ROOT}/3rdparty/luasocket-2.0.2/src/wsocket.c )
  else ( BUILD_WINDOWS )
    set ( LUASOCKET_PLAT ${MOAI_ROOT}/3rdparty/luasocket-2.0.2/src/usocket.c )
  endif ( BUILD_WINDOWS )

 include_directories ( ${MOAI_ROOT}/3rdparty/luasocket-2.0.2/src ${LUA_HEADERS})
  
  add_library ( luasocket STATIC
      ${MOAI_ROOT}/3rdparty/luasocket-2.0.2-embed/fullluasocket.c
    ${MOAI_ROOT}/3rdparty/luasocket-2.0.2-embed/luasocketscripts.c
    ${MOAI_ROOT}/3rdparty/luasocket-2.0.2/src/auxiliar.c
    ${MOAI_ROOT}/3rdparty/luasocket-2.0.2/src/buffer.c
    ${MOAI_ROOT}/3rdparty/luasocket-2.0.2/src/except.c
    ${MOAI_ROOT}/3rdparty/luasocket-2.0.2/src/inet.c
    ${MOAI_ROOT}/3rdparty/luasocket-2.0.2/src/io.c
    ${MOAI_ROOT}/3rdparty/luasocket-2.0.2/src/luasocket.c
    ${MOAI_ROOT}/3rdparty/luasocket-2.0.2/src/mime.c
    ${MOAI_ROOT}/3rdparty/luasocket-2.0.2/src/options.c
    ${MOAI_ROOT}/3rdparty/luasocket-2.0.2/src/select.c
    ${MOAI_ROOT}/3rdparty/luasocket-2.0.2/src/tcp.c
    ${MOAI_ROOT}/3rdparty/luasocket-2.0.2/src/timeout.c
    ${MOAI_ROOT}/3rdparty/luasocket-2.0.2/src/udp.c
    ${LUASOCKET_PLAT}
  )
  
  LINK_DIRECTORIES("${LUA_LIB_DIR}")
  target_link_libraries ( luasocket ${LUA_LIB} )

  target_include_directories ( luasocket PUBLIC "$<BUILD_INTERFACE:${LUA_HEADERS}>")
  add_dependencies ( luasocket liblua-static)
  set_target_properties( luasocket PROPERTIES FOLDER Third-Party )

   if ( BUILD_WINDOWS )
     target_link_libraries( luasocket Ws2_32 )
   endif (BUILD_WINDOWS)
install(TARGETS luasocket EXPORT libmoai ARCHIVE DESTINATION lib)