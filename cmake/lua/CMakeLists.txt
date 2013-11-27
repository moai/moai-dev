cmake_minimum_required(VERSION 2.8.5)
project(lua)

# Check to ensure that we are running as part of the
# root CMakeLists.txt file.
if(NOT DEFINED IS_ROOT)
  message(FATAL_ERROR "You must generate the CMake files as part of the root directory.")
endif(NOT DEFINED IS_ROOT)

set(CMAKE_CURRENT_SOURCE_DIR "${ROOT_DIR}/3rdparty/lua-5.1.3/src/")
add_definitions(-DLUA_USE_LINUX)

moai_manual(executable
    ${CMAKE_CURRENT_SOURCE_DIR}/lua.c
)
moai_needs(lua-lib)
target_link_libraries(lua readline ${CMAKE_DL_LIBS})
