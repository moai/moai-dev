cmake_minimum_required(VERSION 2.8.5)
project(expat)

# Check to ensure that we are running as part of the
# root CMakeLists.txt file.
if(NOT DEFINED IS_ROOT)
  message(FATAL_ERROR "You must generate the CMake files as part of the root directory.")
endif(NOT DEFINED IS_ROOT)

add_definitions(-DHAVE_MEMMOVE)
set(${PROJECT_NAME}_UnixInclude "${ROOT_DIR}/3rdparty/expat-2.0.1/lib/" PARENT_SCOPE)
include_directories("${ROOT_DIR}/3rdparty/expat-2.0.1/lib/")
set(CMAKE_CURRENT_SOURCE_DIR "${ROOT_DIR}/3rdparty/expat-2.0.1/lib/")

moai_manual(library
    ${CMAKE_CURRENT_SOURCE_DIR}/xmlparse.c
    ${CMAKE_CURRENT_SOURCE_DIR}/xmlrole.c
    ${CMAKE_CURRENT_SOURCE_DIR}/xmltok.c
    ${CMAKE_CURRENT_SOURCE_DIR}/xmltok_impl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/xmltok_ns.c
)