cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-luaext )

set ( MOAI_LUAEXT_INCLUDES "../../src/;../../3rdparty/;../../src/config-default/;../../3rdparty/mongoose/" )

include_directories ( 
  ${THIRD_PARTY_INCLUDES}
  ${MOAICORE_INCLUDES} 
  ${MOAI_LUAEXT_INCLUDES}
)

set ( CMAKE_CURRENT_SOURCE_DIR "../../src/moai-luaext/" )
file ( GLOB MOAI_LUAEXT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )
add_library ( moai-luaext STATIC ${MOAI_LUAEXT_SOURCE} )
target_link_libraries ( moai-luaext
	third-party
)
