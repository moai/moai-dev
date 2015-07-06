cmake_minimum_required(VERSION 2.8.5)

project(host-sdl)
set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake-modules")

set (LUA_LIB_DIR  "")
set (MOAI_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")

  find_library ( APPLICATION_SERVICES_LIBRARY ApplicationServices )
  find_library ( CORE_SERVICES_LIBRARY CoreServices )
  

if (LIB_PATH)
  get_filename_component(LIB_PATH ${LIB_PATH} ABSOLUTE)
endif(LIB_PATH)

if (HOST_ROOT)
  get_filename_component(HOST_ROOT ${HOST_ROOT} ABSOLUTE)
else (HOST_ROOT)
  set (HOST_ROOT ${MOAI_ROOT}/src)
endif(HOST_ROOT)
  


include(../../cmake-modules/platform-osx.cmake)
include(../../cmake-modules/platform-common.cmake)
include(../../cmake-modules/module-setup.cmake)
include(../../cmake-modules/libmoai-setup.cmake)
install(EXPORT libmoai DESTINATION cmake )


add_subdirectory(../host-modules host-modules)

include_directories ( ${OPENGL_INCLUDE_DIRS} ${HOST_ROOT} )

#fix lua lib path
if (NOT LIB_PATH)
  link_directories(${LUA_LIB_DIR})
endif (NOT LIB_PATH)

# Use all cpp files in directory as source code
file ( GLOB HOST_SDL_SRC "${HOST_ROOT}/host-sdl/*.cpp" "${HOST_ROOT}/host-sdl/*.h" )

find_package ( OpenGL REQUIRED )

add_executable ( moai ${HOST_SDL_SRC} )
install(TARGETS moai DESTINATION bin)

target_link_libraries ( moai 
  host-modules
  moai-sim
  moai-util
  moai-core
  moai-apple
  moai-sdl
  zlcore
  SDL2-static
    ${APPLICATION_SERVICES_LIBRARY}
    ${CORE_SERVICES_LIBRARY}
    ${OPENGL_LIBRARIES}
  )
set_target_properties( moai PROPERTIES
   XCODE_ATTRIBUTE_INSTALL_PATH "$(LOCAL_APPS_DIR)"
   XCODE_ATTRIBUTE_SKIP_INSTALL "NO"
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
  
#64 bit macs should add this to keep luajit happy
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
     set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pagezero_size 10000 -image_base 100000000 ")
endif (CMAKE_SIZEOF_VOID_P EQUAL 8)
  
