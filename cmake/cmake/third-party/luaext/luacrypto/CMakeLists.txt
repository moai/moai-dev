project( luacrypto )

set ( LUAEXT_SOURCES 
  ${MOAI_ROOT}/3rdparty/luacrypto-0.3.2/src/lcrypto.c
)


add_library (luacrypto STATIC ${LUAEXT_SOURCES})
LINK_DIRECTORIES("${LUA_LIB_DIR}")
target_link_libraries ( luacrypto crypto ${LUA_LIB} )
add_dependencies ( luacrypto liblua-static)
target_include_directories ( luacrypto PUBLIC "$<BUILD_INTERFACE:${LUA_HEADERS}>")
set_target_properties( luacrypto PROPERTIES FOLDER Third-Party )
install(TARGETS luacrypto EXPORT libmoai ARCHIVE DESTINATION lib)
