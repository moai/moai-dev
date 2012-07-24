cmake_minimum_required(VERSION 2.8.5)
project(z)

# Check to ensure that we are running as part of the
# root CMakeLists.txt file.
if(NOT DEFINED IS_ROOT)
  message(FATAL_ERROR "You must generate the CMake files as part of the root directory.")
endif(NOT DEFINED IS_ROOT)

set(${PROJECT_NAME}_UnixInclude "${ROOT_DIR}/3rdparty/zlib-1.2.3/" PARENT_SCOPE)
set(CMAKE_CURRENT_SOURCE_DIR "${ROOT_DIR}/3rdparty/zlib-1.2.3/")

moai_manual(library
    ${CMAKE_CURRENT_SOURCE_DIR}/adler32.c
    ${CMAKE_CURRENT_SOURCE_DIR}/compress.c
    ${CMAKE_CURRENT_SOURCE_DIR}/crc32.c
    ${CMAKE_CURRENT_SOURCE_DIR}/deflate.c
    ${CMAKE_CURRENT_SOURCE_DIR}/gzio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/infback.c
    ${CMAKE_CURRENT_SOURCE_DIR}/inffast.c
    ${CMAKE_CURRENT_SOURCE_DIR}/inftrees.c
    ${CMAKE_CURRENT_SOURCE_DIR}/inflate.c
    ${CMAKE_CURRENT_SOURCE_DIR}/trees.c
    ${CMAKE_CURRENT_SOURCE_DIR}/uncompr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/zutil.c
)