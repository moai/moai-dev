cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-chipmunk )

if ( MOAI_CHIPMUNK )
set ( CMAKE_CURRENT_SOURCE_DIR "../../src/moai-chipmunk/" )

set ( MOAI_CHIPMUNK_INCLUDES 
  ${THIRD_PARTY_INCLUDES}
  "../../src/"
  "../../src/config-default/"

 CACHE INTERNAL "doc string" )

# Use all cpp files in directory as source code
file ( GLOB MOAI_CHIPMUNK_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )

include_directories ( ${MOAI_CHIPMUNK_INCLUDES} )

add_library ( moai-chipmunk STATIC ${MOAI_CHIPMUNK_SRC} )

endif ( MOAI_CHIPMUNK )
