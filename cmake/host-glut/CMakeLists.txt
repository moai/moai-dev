cmake_minimum_required(VERSION 2.8.5)
project(host-glut)

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

set ( INCLUDES_MOAIHOSTGLUT 
  "${MOAI_ROOT}/src/"
  "${MOAI_ROOT}/src/config-default/"
  "${MOAI_ROOT}/src/host-glut/"
  ${OPENGL_INCLUDE_DIRS}
  ${GLUT_INCLUDE_DIRS}
  ${THIRD_PARTY_INCLUDES}
  CACHE INTERNAL "doc string" )


include_directories ( ${INCLUDES_MOAIHOSTGLUT} )
# link_directories (
#   "third-party"
# )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/host-glut" )
# set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )

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
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
  )
endif ( BUILD_LINUX )

if ( BUILD_OSX )
  target_link_libraries ( moai 
    ${APPLICATION_SERVICES_LIBRARY}
    ${CORE_SERVICES_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
  )
endif ( BUILD_OSX )

target_link_libraries ( moai 
	ssl
  third-party 
)

if ( MOAI_UNTZ AND BUILD_LINUX )
  target_link_libraries ( moai ${SDL_LIBRARY} )
endif ( MOAI_UNTZ AND BUILD_LINUX )

