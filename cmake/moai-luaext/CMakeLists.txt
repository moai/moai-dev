cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-luaext )

if (MOAI_LUAEXT)

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DAKU_WITH_LUAEXT=1"  PARENT_SCOPE )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAKU_WITH_LUAEXT=1"  PARENT_SCOPE )


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
file ( GLOB MOAI_LUAEXT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )
add_library ( moai-luaext STATIC ${MOAI_LUAEXT_SOURCE} )
target_link_libraries ( moai-luaext
	third-party
)

else (MOAI_LUAEXT)
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DAKU_WITH_LUAEXT=0"  PARENT_SCOPE )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAKU_WITH_LUAEXT=0"  PARENT_SCOPE)
endif(MOAI_LUAEXT)