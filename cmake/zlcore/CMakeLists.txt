cmake_minimum_required(VERSION 2.8.5)
project(zlcore)

# Check to ensure that we are running as part of the
# root CMakeLists.txt file.
if(NOT DEFINED IS_ROOT)
  message(FATAL_ERROR "You must generate the CMake files as part of the root directory.")
endif(NOT DEFINED IS_ROOT)

set(${PROJECT_NAME}_UnixInclude "${ROOT_DIR}/src/zlcore/" PARENT_SCOPE)
include_directories("${ROOT_DIR}/src/")
set(CMAKE_CURRENT_SOURCE_DIR "${ROOT_DIR}/src/zlcore/")

moai_manual(library
    ${CMAKE_CURRENT_SOURCE_DIR}/ZLDirectoryItr.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ZLFile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ZLFileSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ZLVirtualPath.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ZLZipArchive.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ZLZipStream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/zl_mutex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/zl_util.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/zl_vfscanf.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/zlcore-pch.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/zlcore.cpp
)
moai_needs(tlsf)