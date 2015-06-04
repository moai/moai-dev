cmake_minimum_required(VERSION 2.8.5)

project(host-sdl)
set (LUA_LIB_DIR  "")
set (MOAI_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")
#normalizes slashes to forward slashes for windows
if (LIB_PATH)
   get_filename_component(LIB_PATH ${LIB_PATH} ABSOLUTE)
endif (LIB_PATH)
if (HOST_ROOT)
   get_filename_component(HOST_ROOT ${HOST_ROOT} ABSOLUTE) 
else (HOST_ROOT)
	set(HOST_ROOT ${MOAI_ROOT}/src)
endif (HOST_ROOT)

get_filename_component(MY_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake-modules" ABSOLUTE) 
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${MY_SOURCE}")

include(platform-windows)
include(platform-common)
include(module-setup)



include(libmoai-setup)


add_subdirectory(../host-modules host-modules)

find_package ( OpenGL REQUIRED )
include_directories ( ${OPENGL_INCLUDE_DIRS} ${HOST_ROOT} )

#fix lua lib path
if (NOT LIB_PATH)
	link_directories(${LUA_LIB_DIR})
endif (NOT LIB_PATH)

# Use all cpp files in directory as source code
file ( GLOB HOST_SDL_SRC "${HOST_ROOT}/host-sdl/*.cpp" "${HOST_ROOT}/host-sdl/*.h" )

add_executable ( moai ${HOST_SDL_SRC} )
install(TARGETS moai DESTINATION bin)


message("opengl libraries ${OPENGL_LIBRARIES}")


target_link_libraries ( moai 
  host-modules
  moai-sim
  moai-util
  moai-core
  moai-sdl
  zlcore
  SDL2-static   
  ${OPENGL_LIBRARIES}
  winmm
)


