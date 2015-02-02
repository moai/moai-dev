  project( luasql )

 set ( LUAEXT_SOURCES 
      ${MOAI_ROOT}/3rdparty/luasql-2.2.0/src/ls_sqlite3.c
      ${MOAI_ROOT}/3rdparty/luasql-2.2.0/src/luasql.c
      
    )

 add_library (luasql STATIC ${LUAEXT_SOURCES})
 LINK_DIRECTORIES("${LUA_LIB_DIR}")
 target_link_libraries ( luasql sqlite3 ${LUA_LIB})
 add_dependencies ( luasql liblua-static)
set_target_properties( luasql PROPERTIES FOLDER Third-Party )

 target_include_directories ( luasql PUBLIC "$<BUILD_INTERFACE:${LUA_HEADERS}>")
 install(TARGETS luasql EXPORT libmoai ARCHIVE DESTINATION lib)