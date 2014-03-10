cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-luaext )

set ( MOAI_LUAEXT_INCLUDES "${MOAI_ROOT}/src/;${MOAI_ROOT}/3rdparty/;${MOAI_ROOT}/src/config-default/;${MOAI_ROOT}/3rdparty/mongoose/" )


if ( BUILD_ANDROID )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -include ${MOAI_ROOT}/src/zl-vfs/zl_replace.h" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include ${MOAI_ROOT}/src/zl-vfs/zl_replace.h" )
endif ( BUILD_ANDROID )

include_directories ( 
  ${MOAICORE_INCLUDES} 
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

if (MOAI_CRYPTO) 
  target_link_libraries ( moai-luaext luacrypto )
endif (MOAI_CRYPTO)

if (MOAI_SQLITE3)
  target_link_libraries ( moai-luaext  luasql )
endif (MOAI_SQLITE3)
  
set_target_properties(moai-luaext PROPERTIES FOLDER Modules)  
