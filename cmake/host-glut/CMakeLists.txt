cmake_minimum_required(VERSION 2.8.5)

project(host-glut)

if ( BUILD_LINUX )
  find_package ( OpenGL REQUIRED )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_OS_LINUX -DFREEGLUT_STATIC" )
endif ( BUILD_LINUX )

if ( BUILD_WINDOWS )
  find_package( OpenGL REQUIRED )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DFREEGLUT_STATIC -DFREEGLUT_LIB_PRAGMAS=0" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFREEGLUT_STATIC -DFREEGLUT_LIB_PRAGMAS=0" )
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
)

include_directories ( ${INCLUDES_MOAIHOSTGLUT} )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/host-glut" )

# Use all cpp files in directory as source code
file ( GLOB HOST_GLUT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Glut*.cpp" )
file ( GLOB HOST_GLUT_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Glut*.h" )

add_executable ( moai ${HOST_GLUT_SOURCES} ${HOST_GLUT_HEADERS} )

target_link_libraries ( moai 
  host-modules
  moai-sim
  moai-util
  moai-core
  zlcore
)

if ( BUILD_WINDOWS )
  target_link_libraries ( moai 
    freeglutstatic
    winmm
  )
endif (BUILD_WINDOWS)

if ( BUILD_LINUX )
  target_link_libraries ( moai 
    pthread 
    rt
    freeglutstatic
    Xi
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
  
  if (MOAI_LUAJIT)
     set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pagezero_size 10000 -image_base 100000000 ")
  endif (MOAI_LUAJIT)

endif ( BUILD_OSX )

