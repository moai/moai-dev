cmake_minimum_required(VERSION 2.8.5)
project(http-host)

set ( HTTPHOST_INCLUDES "../../src/;../../src/hosts/" CACHE INTERNAL "doc string" )

include_directories ( 
  ${HTTPHOST_INCLUDES} 
  ${MOAICORE_INCLUDES}
  ${LUA_INCLUDES} 
  ${ZLIB_INCLUDES}
  ${GLEW_INCLUDES}
   )

set ( CMAKE_CURRENT_SOURCE_DIR "../../src/hosts" )

add_executable ( http-host 
  ${CMAKE_CURRENT_SOURCE_DIR}/HTTPHost.cpp
)

target_link_libraries ( http-host moai-core zlcore uslscore third-party )
