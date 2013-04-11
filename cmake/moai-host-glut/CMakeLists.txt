cmake_minimum_required(VERSION 2.8.5)
project(moai-host-glut)

set ( INCLUDES_MOAIHOSTGLUT "../../src/;../../src/hosts/" CACHE INTERNAL "doc string" )

include_directories ( ${INCLUDES_MOAIHOSTGLUT} )
# link_directories (
#   "third-party"
# )

set ( CMAKE_CURRENT_SOURCE_DIR "../../src/hosts" )
# set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )

if ( BUILD_OSX )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework OpenGL -framework GLUT" )
  
endif ( BUILD_OSX )

add_executable ( moai
  ${CMAKE_CURRENT_SOURCE_DIR}/GlutHost.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/GlutHostMain.cpp
)

target_link_libraries ( moai aku moai-core zlcore uslscore third-party )
