cmake_minimum_required(VERSION 2.8.5)
project(host-test)

if ( BUILD_LINUX )
  find_package ( OpenGL REQUIRED )
  find_package ( GLUT REQUIRED )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_OS_LINUX" )
endif ( BUILD_LINUX )

if ( BUILD_OSX )
  find_library ( APPLICATION_SERVICES_LIBRARY ApplicationServices )
  find_library ( CORE_SERVICES_LIBRARY CoreServices )
  find_package ( OpenGL REQUIRED )
  find_package ( GLUT REQUIRED )
endif ( BUILD_OSX )

set ( INCLUDES_MOAIHOSTTEST 
  "${MOAI_ROOT}/src/"
  "${MOAI_ROOT}/src/config-default/"
  "${MOAI_ROOT}/src/host-test/"
  ${OPENGL_INCLUDE_DIRS}
  ${GLUT_INCLUDE_DIRS}
  ${THIRD_PARTY_INCLUDES}
  )


include_directories ( ${INCLUDES_MOAIHOSTTEST} )
# link_directories (
#   "third-party"
# )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/host-test" )

# Use all cpp files in directory as source code
file ( GLOB HOST_TEST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

add_executable ( moai-test-runner ${HOST_TEST_SRC} )

target_link_libraries ( moai-test-runner
  moai-sim
  moai-test
  moai-util
  moai-core
  zlcore
)

if ( MOAI_HTTP_CLIENT )
  target_link_libraries ( moai-test-runner moai-http-client )
endif ( MOAI_HTTP_CLIENT )

if ( MOAI_BOX2D )
  target_link_libraries ( moai-test-runner moai-box2d )
endif ( MOAI_BOX2D )

if ( MOAI_CHIPMUNK )
  target_link_libraries ( moai-test-runner moai-chipmunk )
endif ( MOAI_CHIPMUNK )

if ( MOAI_UNTZ  )
  target_link_libraries ( moai-test-runner moai-untz)
endif ( MOAI_UNTZ )

if ( BUILD_LINUX )
  target_link_libraries ( moai-test-runner
    pthread 
    rt
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
  )
endif ( BUILD_LINUX )

if ( BUILD_OSX )
  target_link_libraries ( moai-test-runner 
    ${APPLICATION_SERVICES_LIBRARY}
    ${CORE_SERVICES_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
  )
endif ( BUILD_OSX )

target_link_libraries ( moai-test-runner
  third-party 
)

if ( MOAI_UNTZ AND BUILD_LINUX )
  target_link_libraries ( moai-test-runner ${SDL_LIBRARY})
endif ( MOAI_UNTZ AND BUILD_LINUX )

