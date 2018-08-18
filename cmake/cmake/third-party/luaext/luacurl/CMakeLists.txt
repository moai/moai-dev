 project( luacurl )
 
 set ( LUAEXT_SOURCES
      ${MOAI_ROOT}/3rdparty/luacurl-1.2.1/luacurl.c
 )
 
 add_library (luacurl STATIC ${LUAEXT_SOURCES})
 LINK_DIRECTORIES("${LUA_LIB_DIR}")
 target_link_libraries ( luacurl curl ${LUA_LIB} )
 add_dependencies ( luacurl liblua-static)
 target_include_directories ( luacurl PUBLIC "$<BUILD_INTERFACE:${LUA_HEADERS}>")
 set_target_properties( luacurl PROPERTIES FOLDER Third-Party )
 install(TARGETS luacurl EXPORT libmoai ARCHIVE DESTINATION lib)