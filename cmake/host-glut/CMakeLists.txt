cmake_minimum_required(VERSION 2.8.5)
project(host-glut)

if ( BUILD_LINUX )
  find_package ( OpenGL REQUIRED )
  find_package ( GLUT REQUIRED )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_OS_LINUX" )
endif ( BUILD_LINUX )

set ( INCLUDES_MOAIHOSTGLUT 
  "../../src/"
  "../../src/config-default/"
  "../../src/host-glut/"
  ${OPENGL_INCLUDE_DIRS}
  ${GLUT_INCLUDE_DIRS}
  ${THIRD_PARTY_INCLUDES}
  CACHE INTERNAL "doc string" )


include_directories ( ${INCLUDES_MOAIHOSTGLUT} )
# link_directories (
#   "third-party"
# )

set ( CMAKE_CURRENT_SOURCE_DIR "../../src/host-glut" )
# set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )

if ( BUILD_OSX )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework OpenGL -framework GLUT -framework CoreServices -framework ApplicationServices" )
endif ( BUILD_OSX )

add_executable ( moai
  ${CMAKE_CURRENT_SOURCE_DIR}/GlutHost.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/GlutHostMain.cpp
)

target_link_libraries ( moai 
  moai-sim
  moai-util
  moai-core
  zlcore
)

if ( MOAI_BOX2D )
  target_link_libraries ( moai moai-box2d )
endif ( MOAI_BOX2D )

if ( MOAI_CHIPMUNK )
  target_link_libraries ( moai moai-chipmunk )
endif ( MOAI_CHIPMUNK )

if ( MOAI_UNTZ AND BUILD_LINUX )
  target_link_libraries ( moai moai-untz )
endif ( MOAI_UNTZ AND BUILD_LINUX )

if ( BUILD_LINUX )
target_link_libraries ( moai 
  pthread 
  rt
  ${OpenGL_LIBRARIES}
  ${GLUT_LIBRARIES}
)
endif ( BUILD_LINUX )

target_link_libraries ( moai 
	ssl
  third-party 
)

if ( MOAI_UNTZ AND BUILD_LINUX )
  target_link_libraries ( moai ${SDL_LIBRARY} )
endif ( MOAI_UNTZ AND BUILD_LINUX )

