cmake_minimum_required(VERSION 2.8.5)



project(host-glut)



if ( BUILD_LINUX )
  add_subdirectory ( freeglut )
  find_package ( OpenGL REQUIRED )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_OS_LINUX" )
endif ( BUILD_LINUX )

if ( BUILD_WINDOWS )
   add_subdirectory ( freeglut )
   find_package( OpenGL REQUIRED )
   set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static -Os" )
   set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -Os" )
endif ( BUILD_WINDOWS )

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
  ${FREEGLUT_INCLUDES}
  ${THIRD_PARTY_INCLUDES}
  )


include_directories ( ${INCLUDES_MOAIHOSTGLUT} )
# link_directories (
#   "third-party"
# )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/host-glut" )
# set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )

add_executable ( moai
  ${CMAKE_CURRENT_SOURCE_DIR}/GlutHost.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/GlutHostMain.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../host-modules/aku_modules.cpp
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

if (MOAI_HTTP_CLIENT)
  target_link_libraries ( moai moai-http-client )
endif ( MOAI_HTTP_CLIENT)

if ( MOAI_UNTZ )
  target_link_libraries ( moai moai-untz )
endif ( MOAI_UNTZ )


if ( BUILD_WINDOWS )
  target_link_libraries ( moai 
    freeglut-static
    ${OPENGL_LIBRARIES}
    winmm
  )
endif (BUILD_WINDOWS)

if ( BUILD_LINUX )
target_link_libraries ( moai 
  pthread 
  rt
  freeglut-static
  Xi
  ${OPENGL_LIBRARIES}
)
endif ( BUILD_LINUX )

if ( BUILD_OSX )
  target_link_libraries ( moai 
    ${APPLICATION_SERVICES_LIBRARY}
    ${CORE_SERVICES_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
  )

  macro(ADD_FRAMEWORK fwname appname)
    find_library(FRAMEWORK_${fwname}
        NAMES ${fwname}
        PATHS ${CMAKE_SYSTEM_FRAMEWORK_PATH}
        NO_DEFAULT_PATH)
    if( ${FRAMEWORK_${fwname}} STREQUAL FRAMEWORK_${fwname}-NOTFOUND)
        MESSAGE(ERROR ": Framework ${fwname} not found")
    else()
        TARGET_LINK_LIBRARIES(${appname} "${FRAMEWORK_${fwname}}/${fwname}")
        MESSAGE(STATUS "Framework ${fwname} found at ${FRAMEWORK_${fwname}}")
    endif()
endmacro(ADD_FRAMEWORK)

macro(ADD_FRAMEWORKS appname fwnames)
  FOREACH(FW ${fwnames})
    ADD_FRAMEWORK(${FW} ${appname})
  ENDFOREACH(FW)
endmacro(ADD_FRAMEWORKS)

set( FWORKS
                  AudioToolbox
                  AudioUnit
                  CoreAudio
                  CoreGraphics
                  CoreLocation
                  Foundation
                  GameKit
                  QuartzCore
                  StoreKit
                  SystemConfiguration
                  )

ADD_FRAMEWORKS(moai "${FWORKS}")

endif ( BUILD_OSX )


target_link_libraries ( moai 
  third-party
)
if (MOAI_LUAEXT)
 target_link_libraries ( moai moai-luaext )
endif (MOAI_LUAEXT)

if ( MOAI_UNTZ AND BUILD_LINUX )
  target_link_libraries ( moai ${SDL_LIBRARY} )
endif ( MOAI_UNTZ AND BUILD_LINUX )

