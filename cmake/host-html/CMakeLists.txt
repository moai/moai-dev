cmake_minimum_required(VERSION 2.8.5)


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

add_library ( moaihtml SHARED
  ${CMAKE_CURRENT_SOURCE_DIR}/HtmlHost.cpp
)
set( MOAIHTMLOUTPUT "test")
get_target_property( MOAIHTMLOUTPUT moaihtml LOCATION)
set(EXPORTED_FUNCTIONS "['_RefreshContext','_Cleanup','_onKeyDown','_onKeyUp','_onMouseButton','_onMouseDrag','_onMouseMove','_onPaint','_onReshape', '_onTimer']" )

message(STATUS "${MOAIHTMLOUTPUT} -s EXPORTED_FUNCTIONS=\"${EXPORTED_FUNCTIONS}\" -o moaihtml.js" )
add_custom_command( TARGET moaihtml
					POST_BUILD
                    COMMAND ${EMSCRIPTEN_TOOLS}/emcc ARGS ${MOAIHTMLOUTPUT} -s LINKABLE=1 -s EXPORTED_FUNCTIONS="${EXPORTED_FUNCTIONS}"  -o moaihtml.js
					VERBATIM
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

target_link_libraries ( moaihtml
  third-party 
)
