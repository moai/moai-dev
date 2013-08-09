cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-iphone )

set ( MOAI_IPHONE_INCLUDES 
  ${THIRD_PARTY_INCLUDES}
  "../../src/"
  "../../src/config-default/"

 CACHE INTERNAL "doc string" )

include_directories ( 
  ${MOAI_IPHONE_INCLUDES} 
)

set ( CMAKE_CURRENT_SOURCE_DIR "../../src/moai-iphone/" )

# Use all cpp files in directory as source code
file ( GLOB MOAI_IPHONE_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.mm" )

include_directories ( ${MOAI_IPHONE_INCLUDES} )

add_library ( moai-iphone STATIC ${MOAI_IPHONE_SRC} "${CMAKE_CURRENT_SOURCE_DIR}/Reachability.m" )