project( luacrypto )

set ( LUAEXT_SOURCES ${LUAEXT_SOURCES}
  ${MOAI_ROOT}/3rdparty/luacrypto-0.2.0/src/lcrypto.c
)


add_library (luacrypto STATIC ${LUAEXT_SOURCES})
target_link_libraries ( luacrypto crypto liblua-static )
target_include_directories ( luacrypto PUBLIC ${LUA_HEADERS})
