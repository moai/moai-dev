cmake_minimum_required(VERSION 2.8.5)
project(http-host)

set ( HTTPHOST_INCLUDES "${MOAI_ROOT}/src/;${MOAI_ROOT}/src/hosts/"  )

include_directories ( 
  ${HTTPHOST_INCLUDES} 
  ${MOAICORE_INCLUDES}
  ${MOAIEXT_SERVER_INCLUDES}
  ${LUA_INCLUDES} 
  ${ZLIB_INCLUDES}
  ${GLEW_INCLUDES}
   )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/host-server/" )

# Use all cpp files in directory as source code
file ( GLOB HOST_HTTP_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

add_executable ( http-host ${HOST_HTTP_SRC} )

target_link_libraries ( http-host moai-core moaiext-server zlcore uslscore third-party rt dl pthread )

