cmake_minimum_required(VERSION 2.8.5)

project(host-sdl)

set (HOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../")

get_filename_component(MY_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${MY_SOURCE}")

if (LIB_PATH)
  get_filename_component(LIB_PATH ${LIB_PATH} ABSOLUTE)
else(LIB_PATH)
   message(fatal "Must provide a library path via -DLIB_PATH=<path to moai libs and includes>")
endif(LIB_PATH)

find_package ( OpenGL REQUIRED )

set ( VFS_INCLUDE "-I${LIB_PATH}/include -include ${LIB_PATH}/include/zl-vfs/zl_replace.h")
set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DPOSIX -std=gnu99 -DNDEBUG -DHAVE_MEMMOVE ${VFS_INCLUDE}" )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DPOSIX2 ${VFS_INCLUDE}" )

#link lib moai
include(${LIB_PATH}/cmake/libmoai.cmake)

link_directories(${LIB_PATH}/lib)

find_package ( XCB QUIET  )

if ( NOT XCB_XCB_FOUND )
  message(  "You need to install libxcb-dev" )
endif ( NOT XCB_XCB_FOUND )

if ( NOT XCB_UTIL_FOUND )
  message(  "You need to install libxcb-util0-dev" )
endif ( NOT XCB_UTIL_FOUND )

if ( NOT XCB_RANDR_FOUND )
  message(  "You need to install libxcb-randr0-dev" )
endif ( NOT XCB_RANDR_FOUND )

if (NOT (XCB_XCB_FOUND AND XCB_RANDR_FOUND AND XCB_UTIL_FOUND))
  message (FATAL_ERROR "Please install the missing libs and try again")
endif (NOT (XCB_XCB_FOUND AND XCB_RANDR_FOUND AND XCB_UTIL_FOUND))

find_package(X11)
   
FIND_LIBRARY(X11_Xxf86vm_LIB Xxf86vm ${X11_LIB_SEARCH_PATH})
FIND_LIBRARY(XI_LIBRARY Xi ${X11_LIB_SEARCH_PATH})
MARK_AS_ADVANCED(X11_Xxf86vm_LIB XI_LIBRARY)



add_subdirectory(host-modules)

include_directories ( ${OPENGL_INCLUDE_DIRS} ${HOST_ROOT} )


# Use all cpp files in directory as source code
file ( GLOB HOST_SDL_SRC "${HOST_ROOT}/host-sdl/*.cpp" "${HOST_ROOT}/host-sdl/*.h" )

add_executable ( moai ${HOST_SDL_SRC} )

target_link_libraries ( moai 
  host-modules
  moai-sim
  moai-util
  moai-core
  zlcore
  SDL2-static
  pthread 
  rt
 ${XI_LIBRARY}
  ${OPENGL_LIBRARIES}
  ${X11_Xxf86vm_LIB}
  ${XCB_XCB_LIBRARIES}
  ${XCB_RANDR_LIBRARIES}
  ${XCB_UTIL_LIBRARIES}
)
