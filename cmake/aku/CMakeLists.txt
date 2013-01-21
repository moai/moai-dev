cmake_minimum_required(VERSION 2.8.5)
project(aku)

# Check to ensure that we are running as part of the
# root CMakeLists.txt file.
if(NOT DEFINED IS_ROOT)
  message(FATAL_ERROR "You must generate the CMake files as part of the root directory.")
endif(NOT DEFINED IS_ROOT)

set(${PROJECT_NAME}_UnixInclude "${ROOT_DIR}/src/aku/" PARENT_SCOPE)
include_directories("${ROOT_DIR}/src/")
include_directories("${ROOT_DIR}/3rdparty/")
include_directories("${ROOT_DIR}/src/config-default/")
set(CMAKE_CURRENT_SOURCE_DIR "${ROOT_DIR}/src/aku/")

moai_manual(library
    #${CMAKE_CURRENT_SOURCE_DIR}/AKU-audiosampler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AKU-debugger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AKU-fmod-designer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AKU-fmod-ex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AKU-luaext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AKU-particles.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AKU-test.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/AKU-untz.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AKU.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp
)
moai_needs(expat)
moai_needs(glew)
moai_needs(contrib)
moai_needs(freetype)
moai_needs(tinyxml)
moai_needs(box2d)
moai_needs(chipmunk)
moai_needs(jansson)
moai_needs(moaicore)
moai_needs(moaiext-test)
moai_needs(lua-lib)
