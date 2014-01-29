 project( luafilesystem )

 set ( LUAEXT_SOURCES 
     ${MOAI_ROOT}/3rdparty/luafilesystem-1.5.0/src/lfs.c
  )

 add_library (luafilesystem STATIC ${LUAEXT_SOURCES})
 target_link_libraries ( luafilesystem liblua-static )
 target_include_directories ( luafilesystem PUBLIC ${LUA_HEADERS})