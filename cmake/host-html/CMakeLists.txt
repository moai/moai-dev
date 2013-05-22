cmake_minimum_required(VERSION 2.8.5)

SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_OS_HTML" )
project(host-html)

# these must be set *after* project()
    SET(CMAKE_AR                  "${EMSCRIPTEN_TOOLS}/emar")
    SET(CMAKE_RANLIB              "${EMSCRIPTEN_TOOLS}/emranlib")




  


set ( INCLUDES_MOAIHOSTGLUT 
  "../../src/"
  "../../src/config-default/"
  "../../src/host-html/"
  ${OPENGL_INCLUDE_DIRS}
  CACHE INTERNAL "doc string" )


include_directories ( ${INCLUDES_MOAIHOSTGLUT} )
# link_directories (
#   "third-party"
# )

set ( CMAKE_CURRENT_SOURCE_DIR "../../src/host-html" )
# set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )

if ( BUILD_OSX )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework OpenGL" )
endif ( BUILD_OSX )

add_library ( moaihtml STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/HtmlHost.cpp
)

target_link_libraries ( moaihtml
  moai-sim
  moai-util
  moai-core
  zlcore
)

if ( MOAI_BOX2D )
  target_link_libraries ( moaihtml moai-box2d )
endif ( MOAI_BOX2D )

if ( MOAI_CHIPMUNK )
  target_link_libraries ( moaihtml moai-chipmunk )
endif ( MOAI_CHIPMUNK )

if ( BUILD_LINUX )
target_link_libraries ( moaihtml
  pthread 
  rt
  ${OpenGL_LIBRARIES}
)
endif ( BUILD_LINUX )

target_link_libraries ( moaihtml
  third-party 
)
