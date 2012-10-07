cmake_minimum_required(VERSION 2.8.5)
project(glewinfo)

# Check to ensure that we are running as part of the
# root CMakeLists.txt file.
if(NOT DEFINED IS_ROOT)
  message(FATAL_ERROR "You must generate the CMake files as part of the root directory.")
endif(NOT DEFINED IS_ROOT)

include_directories("${ROOT_DIR}/3rdparty/glew-1.5.6/include")
set(CMAKE_CURRENT_SOURCE_DIR "${ROOT_DIR}/3rdparty/glew-1.5.6/src/")

moai_manual(executable
    ${CMAKE_CURRENT_SOURCE_DIR}/glewinfo.c
)
moai_needs(glew)