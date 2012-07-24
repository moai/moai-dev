cmake_minimum_required(VERSION 2.8.5)
project(glew)

# Check to ensure that we are running as part of the
# root CMakeLists.txt file.
if(NOT DEFINED IS_ROOT)
  message(FATAL_ERROR "You must generate the CMake files as part of the root directory.")
endif(NOT DEFINED IS_ROOT)

set(${PROJECT_NAME}_UnixInclude "${ROOT_DIR}/3rdparty/glew-1.5.6/include" PARENT_SCOPE)
include_directories("${ROOT_DIR}/3rdparty/glew-1.5.6/include")
set(CMAKE_CURRENT_SOURCE_DIR "${ROOT_DIR}/3rdparty/glew-1.5.6/src/")

moai_manual(library
    ${CMAKE_CURRENT_SOURCE_DIR}/glew.c
)
target_link_libraries(glew ${OPENGL_LIBRARIES})