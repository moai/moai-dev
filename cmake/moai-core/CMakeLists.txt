cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-core )

if ( BUILD_ANDROID )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -include ${CMAKE_CURRENT_SOURCE_DIR}/../../src/zl-vfs/zl_replace.h" )
   set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include ${CMAKE_CURRENT_SOURCE_DIR}/../../src/zl-vfs/zl_replace.h" )
endif ( BUILD_ANDROID )

set ( MOAI_CORE_INCLUDES 
  ${THIRD_PARTY_INCLUDES}
  "../../src/"
  "../../src/config-default/"

 CACHE INTERNAL "doc string" )

include_directories ( 
  ${MOAI_CORE_INCLUDES} 
)

set ( CMAKE_CURRENT_SOURCE_DIR "../../src/moai-core/" )

# Use all cpp files in directory as source code
file ( GLOB MOAI_CORE_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )

include_directories ( ${MOAI_CORE_INCLUDES} )

add_library ( moai-core STATIC ${MOAI_CORE_SRC} )
target_link_libraries ( moai-core zlcore )
