cmake_minimum_required(VERSION 2.8.5)
project(host-test)

if ( BUILD_LINUX )
  find_package ( OpenGL REQUIRED )
  find_package ( GLUT REQUIRED )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_OS_LINUX" )
endif ( BUILD_LINUX )

set ( INCLUDES_MOAIHOSTTEST 
  "../../src/"
  "../../src/config-default/"
  "../../src/host-test/"
  ${OPENGL_INCLUDE_DIRS}
  ${GLUT_INCLUDE_DIRS}
  ${THIRD_PARTY_INCLUDES}
  CACHE INTERNAL "doc string" )


include_directories ( ${INCLUDES_MOAIHOSTTEST} )
# link_directories (
#   "third-party"
# )

set ( CMAKE_CURRENT_SOURCE_DIR "../../src/host-test" )
# set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )

if ( BUILD_OSX )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework OpenGL -framework GLUT -framework CoreServices -framework ApplicationServices" )
endif ( BUILD_OSX )

add_executable ( moai-test-runner
  ${CMAKE_CURRENT_SOURCE_DIR}/GlutHostTest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/GlutHostTestMain.cpp
)

target_link_libraries ( moai-test-runner
  moai-sim
  moai-util
  moai-core
  moai-test
  zlcore
)

if ( MOAI_BOX2D )
  target_link_libraries ( moai-test-runner moai-box2d )
endif ( MOAI_BOX2D )

if ( MOAI_CHIPMUNK )
  target_link_libraries ( moai-test-runner moai-chipmunk )
endif ( MOAI_CHIPMUNK )

if ( BUILD_LINUX )
target_link_libraries ( moai-test-runner
  pthread 
  rt
  ${OpenGL_LIBRARIES}
  ${GLUT_LIBRARIES}
)
endif ( BUILD_LINUX )

target_link_libraries ( moai-test-runner
  third-party 
)
