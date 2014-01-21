cmake_minimum_required(VERSION 2.8.5)

project(host-sdl)


if ( BUILD_OSX )
  find_library ( APPLICATION_SERVICES_LIBRARY ApplicationServices )
  find_library ( CORE_SERVICES_LIBRARY CoreServices )
endif ( BUILD_OSX )

set ( INCLUDES_MOAIHOSTSDL 
  "${MOAI_ROOT}/src/"
  "${MOAI_ROOT}/src/config-default/"
  "${MOAI_ROOT}/src/host-glut/"
  "${MOAI_ROOT}/3rdparty/sdl2-2.0.0/include"
   ${OPENGL_INCLUDE_DIRS}
)

include_directories ( ${INCLUDES_MOAIHOSTSDL} )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/host-sdl" )

add_executable ( moai
  ${CMAKE_CURRENT_SOURCE_DIR}/SDLHost.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/SDLHostMain.cpp
)

target_link_libraries ( moai 
  host-modules
  moai-sim
  moai-util
  moai-core
  zlcore
  SDL2-static
)

if ( BUILD_WINDOWS )
  target_link_libraries ( moai 
    SDL2-static
    ${OPENGL_LIBRARIES}
    winmm
  )
endif (BUILD_WINDOWS)

if ( BUILD_LINUX )
  target_link_libraries ( moai 
    pthread 
    rt
  )
endif ( BUILD_LINUX )

if ( BUILD_OSX )
  target_link_libraries ( moai 
    ${APPLICATION_SERVICES_LIBRARY}
    ${CORE_SERVICES_LIBRARY}
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
