cmake_minimum_required(VERSION 2.8.5)
project(http-host)

set ( HTTPHOST_INCLUDES "${MOAI_ROOT}/src/;${MOAI_ROOT}/src/hosts/" CACHE INTERNAL "doc string" )

include_directories ( 
  ${HTTPHOST_INCLUDES} 
  ${MOAICORE_INCLUDES}
  ${MOAIEXT_SERVER_INCLUDES}
  ${LUA_INCLUDES} 
  ${ZLIB_INCLUDES}
  ${GLEW_INCLUDES}
   )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/hosts" )

add_executable ( http-host 
  ${CMAKE_CURRENT_SOURCE_DIR}/ServerHost.cpp
)

target_link_libraries ( http-host moai-core moaiext-server zlcore uslscore third-party rt dl pthread )

