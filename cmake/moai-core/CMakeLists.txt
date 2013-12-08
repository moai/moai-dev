cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-core )

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
