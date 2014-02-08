cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-test )

set ( MOAI_TEST_INCLUDES 
  ${THIRD_PARTY_INCLUDES}
  "${MOAI_ROOT}/src/"
  "${MOAI_ROOT}/src/config-default/"

 CACHE INTERNAL "doc string" )

include_directories ( 
  ${MOAI_CORE_INCLUDES} 
)

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-test/" )

# Use all cpp files in directory as source code
file ( GLOB MOAI_TEST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )

include_directories ( ${MOAI_TEST_INCLUDES} )

add_library ( moai-test STATIC ${MOAI_TEST_SRC} )
