cmake_minimum_required(VERSION 2.8.5)
project(png)

# Check to ensure that we are running as part of the
# root CMakeLists.txt file.
if(NOT DEFINED IS_ROOT)
  message(FATAL_ERROR "You must generate the CMake files as part of the root directory.")
endif(NOT DEFINED IS_ROOT)

set(${PROJECT_NAME}_UnixInclude "${ROOT_DIR}/3rdparty/lpng140/" PARENT_SCOPE)
include_directories("${ROOT_DIR}/3rdparty/lpng140/")
set(CMAKE_CURRENT_SOURCE_DIR "${ROOT_DIR}/3rdparty/lpng140/")

moai_manual(library
    #${CMAKE_CURRENT_SOURCE_DIR}/example.c
    ${CMAKE_CURRENT_SOURCE_DIR}/png.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngerror.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngget.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngmem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngpread.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngread.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngrio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngrtran.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngrutil.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngset.c
    #${CMAKE_CURRENT_SOURCE_DIR}/pngtest.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngtrans.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngwio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngwrite.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngwtran.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngwutil.c
)