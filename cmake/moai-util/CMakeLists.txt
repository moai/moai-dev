cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-util )

if ( BUILD_ANDROID )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -include ${MOAI_ROOT}/src/zl-vfs/zl_replace.h" )
   set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include ${MOAI_ROOT}/src/zl-vfs/zl_replace.h" )
endif ( BUILD_ANDROID )

set ( MOAI_UTIL_INCLUDES 
  "${MOAI_ROOT}/src/"
  "${MOAI_ROOT}/src/config-default/"
  )

include_directories ( 
  ${MOAI_UTIL_INCLUDES} 
)

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-util/" )

file ( GLOB MOAI_UTIL_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

include_directories ( ${MOAI_UTIL_INCLUDES} )

add_library ( moai-util STATIC ${MOAI_UTIL_SRC} )

target_link_libraries ( moai-util moai-core zlcore  )
set_target_properties( moai-util PROPERTIES FOLDER Core)

if (MOAI_SFMT)
	target_link_libraries (moai-util sfmt )
endif (MOAI_SFMT)

if ( MOAI_JSON )
	target_link_libraries( moai-util jansson )
endif (MOAI_JSON ) 

if ( MOAI_TINYXML )
   target_link_libraries( moai-util tinyxml )
endif ( MOAI_TINYXML )