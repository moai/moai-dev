cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-luaext )

set ( MOAI_LUAEXT_INCLUDES "${MOAI_ROOT}/src/;${MOAI_ROOT}/3rdparty/;${CMAKE_BINARY_DIR}/libmoai/config;${MOAI_ROOT}/3rdparty/mongoose/" )


if ( BUILD_ANDROID )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -include ${MOAI_ROOT}/src/zl-vfs/zl_replace.h" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include ${MOAI_ROOT}/src/zl-vfs/zl_replace.h" )
endif ( BUILD_ANDROID )

include_directories ( 
  ${MOAI_LUAEXT_INCLUDES}
)

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-luaext/" )
file ( GLOB MOAI_LUAEXT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

add_library ( moai-luaext STATIC ${MOAI_LUAEXT_SOURCE} )

target_link_libraries ( moai-luaext
	moai-core
  luasocket
  luafilesystem
)

if (MOAI_CURL) 
  target_link_libraries ( moai-luaext luacurl )
endif (MOAI_CURL)

if (MOAI_LIBCRYPTO) 
  target_link_libraries ( moai-luaext luacrypto )
endif (MOAI_LIBCRYPTO)

if (MOAI_SQLITE3)
  target_link_libraries ( moai-luaext  luasql )
endif (MOAI_SQLITE3)
  

target_include_directories (moai-luaext PUBLIC
    "$<BUILD_INTERFACE:${MOAI_LUAEXT_INCLUDES}>"
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/moai-luaext>"
)

set_target_properties(moai-luaext PROPERTIES FOLDER Modules)  
install(TARGETS moai-luaext EXPORT libmoai ARCHIVE DESTINATION lib)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include/moai-luaext
          FILES_MATCHING PATTERN "*.h")
