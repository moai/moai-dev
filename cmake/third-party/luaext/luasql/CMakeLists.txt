  project( luasql )

 set ( LUAEXT_SOURCES ${LUAEXT_SOURCES}
      ${MOAI_ROOT}/3rdparty/luasql-2.2.0/src/ls_sqlite3.c
      ${MOAI_ROOT}/3rdparty/luasql-2.2.0/src/luasql.c
      
    )

 add_library (luasql STATIC ${LUAEXT_SOURCES})
 target_link_libraries ( luasql sqlite3 liblua-static )

 target_include_directories ( luasql PUBLIC ${LUA_HEADERS})