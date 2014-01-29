 project( luacurl )
 
 set ( LUAEXT_SOURCES ${LUAEXT_SOURCES}
      ${MOAI_ROOT}/3rdparty/luacurl-1.2.1/luacurl.c
 )
 
 add_library (luacurl STATIC ${LUAEXT_SOURCES})
 target_link_libraries ( luacurl curl liblua-static )
 target_include_directories ( luacurl PUBLIC ${LUA_HEADERS})