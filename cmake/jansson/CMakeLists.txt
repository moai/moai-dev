cmake_minimum_required(VERSION 2.8.5)
project(jansson)

# Check to ensure that we are running as part of the
# root CMakeLists.txt file.
if(NOT DEFINED IS_ROOT)
  message(FATAL_ERROR "You must generate the CMake files as part of the root directory.")
endif(NOT DEFINED IS_ROOT)

set(${PROJECT_NAME}_UnixInclude "${ROOT_DIR}/3rdparty/jansson-2.1/src/" PARENT_SCOPE)
include_directories("${ROOT_DIR}/3rdparty/jansson-2.1/src/")
set(CMAKE_CURRENT_SOURCE_DIR "${ROOT_DIR}/3rdparty/jansson-2.1/src/")

moai_manual(library
    ${CMAKE_CURRENT_SOURCE_DIR}/dump.c
    ${CMAKE_CURRENT_SOURCE_DIR}/error.c
    ${CMAKE_CURRENT_SOURCE_DIR}/hashtable.c
    ${CMAKE_CURRENT_SOURCE_DIR}/load.c
    ${CMAKE_CURRENT_SOURCE_DIR}/memory.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pack_unpack.c
    ${CMAKE_CURRENT_SOURCE_DIR}/strbuffer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/utf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/value.c
)