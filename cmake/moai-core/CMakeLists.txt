cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-core )

if ( BUILD_ANDROID )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -include ${MOAI_ROOT}/src/zl-vfs/zl_replace.h" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include ${MOAI_ROOT}/src/zl-vfs/zl_replace.h" )
endif ( BUILD_ANDROID )

set ( MOAI_CORE_INCLUDES 
  "${MOAI_ROOT}/src/"
  "${MOAI_ROOT}/src/config-default/"
  "${MOAI_ROOT}/src/moai-core"
)

include_directories ( 
  ${MOAI_CORE_INCLUDES} 
)

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-core/" )

# Use all cpp files in directory as source code
file ( GLOB MOAI_CORE_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

include_directories ( ${MOAI_CORE_INCLUDES} )

add_library ( moai-core STATIC ${MOAI_CORE_SRC} )

#get_target_property ( LUA_HEADERS liblua-static INCLUDE_DIRECTORIES )
target_include_directories ( moai-core PUBLIC ${MOAI_CORE_INCLUDES} ${LUA_HEADERS} )

target_link_libraries ( moai-core zlcore ${LUA_LIB})
add_dependencies ( moai-core liblua-static)

set_target_properties(moai-core PROPERTIES FOLDER Core)
