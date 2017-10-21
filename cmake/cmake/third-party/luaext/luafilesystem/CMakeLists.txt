 project( luafilesystem )

 set ( LUAEXT_SOURCES 
     ${MOAI_ROOT}/3rdparty/luafilesystem-1.5.0/src/lfs.c
  )

 add_library (luafilesystem STATIC ${LUAEXT_SOURCES})
 LINK_DIRECTORIES("${LUA_LIB_DIR}")
 target_link_libraries ( luafilesystem ${LUA_LIB} )
 add_dependencies ( luafilesystem liblua-static)
 target_include_directories ( luafilesystem PUBLIC "$<BUILD_INTERFACE:${LUA_HEADERS}>")
 set_target_properties( luafilesystem PROPERTIES FOLDER Third-Party )
 install(TARGETS luafilesystem EXPORT libmoai ARCHIVE DESTINATION lib)