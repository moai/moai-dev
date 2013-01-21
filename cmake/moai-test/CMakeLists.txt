cmake_minimum_required(VERSION 2.8.5)
project(moai-test)

# Check to ensure that we are running as part of the
# root CMakeLists.txt file.
if(NOT DEFINED IS_ROOT)
  message(FATAL_ERROR "You must generate the CMake files as part of the root directory.")
endif(NOT DEFINED IS_ROOT)

set(CMAKE_CURRENT_SOURCE_DIR "${ROOT_DIR}/src/hosts/")
add_definitions(-D__MOAI_LINUX_BUILD -DGLUT_LIBRARY_PATH="${GLUT_INCLUDE_DIR}/GL/glut.h")
include_directories("${ROOT_DIR}/src/")
include_directories("${ROOT_DIR}/src/hosts/")
include_directories(${GLUT_INCLUDE_DIR})

moai_manual(executable
    ${CMAKE_CURRENT_SOURCE_DIR}/GlutHostTest.cpp
)

#moai_needs(moaicore)
moai_needs(aku)
moai_needs(moaiext-test)


target_link_libraries(moai-test ${GLUT_LIBRARY})