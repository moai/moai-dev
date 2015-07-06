cmake_minimum_required(VERSION 2.8.5)

project(host-sdl)
get_filename_component(MY_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake-modules" ABSOLUTE)
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${MY_SOURCE}")

set (LUA_LIB_DIR  "")
set (MOAI_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")


#set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_OS_LINUX " )

if (LIB_PATH)
  get_filename_component(LIB_PATH ${LIB_PATH} ABSOLUTE)
endif(LIB_PATH)

if (HOST_ROOT)
  get_filename_component(HOST_ROOT ${HOST_ROOT} ABSOLUTE)
else (HOST_ROOT)
  set (HOST_ROOT ${MOAI_ROOT}/src)
endif(HOST_ROOT)

  

include(platform-linux)
include(platform-common)
include(module-setup)
include(libmoai-setup)
install(EXPORT libmoai DESTINATION cmake )

add_subdirectory(../host-modules host-modules)


find_package ( OpenGL REQUIRED )

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

include_directories ( ${OPENGL_INCLUDE_DIRS} ${HOST_ROOT} )

#fix lua lib path
if (NOT LIB_PATH)
  link_directories(${LUA_LIB_DIR})
endif (NOT LIB_PATH)


#set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )

# Use all cpp files in directory as source code
file ( GLOB HOST_SDL_SRC "${HOST_ROOT}/host-sdl/*.cpp" "${HOST_ROOT}/host-sdl/*.h" )

add_executable ( moai ${HOST_SDL_SRC} )
install(TARGETS moai DESTINATION bin)

target_link_libraries ( moai 
  host-modules
  moai-sim
  moai-util
  moai-core
  moai-sdl
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


