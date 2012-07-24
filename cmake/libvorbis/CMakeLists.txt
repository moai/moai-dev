cmake_minimum_required(VERSION 2.8.5)
project(vorbis)

# Check to ensure that we are running as part of the
# root CMakeLists.txt file.
if(NOT DEFINED IS_ROOT)
  message(FATAL_ERROR "You must generate the CMake files as part of the root directory.")
endif(NOT DEFINED IS_ROOT)

set(${PROJECT_NAME}_UnixInclude "${ROOT_DIR}/3rdparty/libvorbis-1.3.2/include/" PARENT_SCOPE)
include_directories("${ROOT_DIR}/3rdparty/libvorbis-1.3.2/lib/")
include_directories("${ROOT_DIR}/3rdparty/libvorbis-1.3.2/include/")
set(CMAKE_CURRENT_SOURCE_DIR "${ROOT_DIR}/3rdparty/libvorbis-1.3.2/lib/")

moai_manual(library
    ${CMAKE_CURRENT_SOURCE_DIR}/analysis.c
    #${CMAKE_CURRENT_SOURCE_DIR}/barkmel.c
    ${CMAKE_CURRENT_SOURCE_DIR}/bitrate.c
    ${CMAKE_CURRENT_SOURCE_DIR}/block.c
    ${CMAKE_CURRENT_SOURCE_DIR}/codebook.c
    ${CMAKE_CURRENT_SOURCE_DIR}/envelope.c
    ${CMAKE_CURRENT_SOURCE_DIR}/floor0.c
    ${CMAKE_CURRENT_SOURCE_DIR}/floor1.c
    ${CMAKE_CURRENT_SOURCE_DIR}/info.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lookup.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lpc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lsp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mapping0.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mdct.c
    ${CMAKE_CURRENT_SOURCE_DIR}/psy.c
    #${CMAKE_CURRENT_SOURCE_DIR}/psytune.c
    ${CMAKE_CURRENT_SOURCE_DIR}/registry.c
    ${CMAKE_CURRENT_SOURCE_DIR}/res0.c
    #${CMAKE_CURRENT_SOURCE_DIR}/sharedbook.c
    ${CMAKE_CURRENT_SOURCE_DIR}/smallft.c
    ${CMAKE_CURRENT_SOURCE_DIR}/synthesis.c
    #${CMAKE_CURRENT_SOURCE_DIR}/tone.c
    ${CMAKE_CURRENT_SOURCE_DIR}/vorbisenc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/vorbisfile.c
    ${CMAKE_CURRENT_SOURCE_DIR}/window.c
)
moai_needs(ogg)