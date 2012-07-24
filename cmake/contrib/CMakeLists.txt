cmake_minimum_required(VERSION 2.8.5)
project(contrib)

# Check to ensure that we are running as part of the
# root CMakeLists.txt file.
if(NOT DEFINED IS_ROOT)
  message(FATAL_ERROR "You must generate the CMake files as part of the root directory.")
endif(NOT DEFINED IS_ROOT)

set(${PROJECT_NAME}_UnixInclude "${ROOT_DIR}/3rdparty/contrib/" PARENT_SCOPE)
set(CMAKE_CURRENT_SOURCE_DIR "${ROOT_DIR}/3rdparty/contrib/")

moai_manual(library
    ${CMAKE_CURRENT_SOURCE_DIR}/utf8.c
)