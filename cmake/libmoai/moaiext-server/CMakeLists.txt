cmake_minimum_required ( VERSION 2.8.5 )
project ( moaiext-server )

set ( MOAIEXT_SERVER_INCLUDES "${MOAI_ROOT}/src/;${MOAI_ROOT}/3rdparty/;${CMAKE_BINARY_DIR}/libmoai/config;${MOAI_ROOT}/3rdparty/mongoose/" CACHE INTERNAL "doc string" )

include_directories ( 
  ${MOAICORE_INCLUDES} 
  ${MOAIEXT_SERVER_INCLUDES} 
  ${MONGOOSE_INCLUDES}
)

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moaiext-server/" )
file ( GLOB SRC_MOAIEXT_SERVER "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )
add_library ( moaiext-server STATIC ${SRC_MOAIEXT_SERVER} )
