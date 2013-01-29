cmake_minimum_required(VERSION 2.8.5)
project(moaiext-test)

# Check to ensure that we are running as part of the
# root CMakeLists.txt file.
if(NOT DEFINED IS_ROOT)
  message(FATAL_ERROR "You must generate the CMake files as part of the root directory.")
endif(NOT DEFINED IS_ROOT)

set(${PROJECT_NAME}_UnixInclude "${ROOT_DIR}/src/moaiext-test/" PARENT_SCOPE)
include_directories("${ROOT_DIR}/src/")
include_directories("${ROOT_DIR}/3rdparty/")
include_directories("${ROOT_DIR}/src/config-default/")
set(CMAKE_CURRENT_SOURCE_DIR "${ROOT_DIR}/src/moaiext-test/")

moai_manual(library
    ${CMAKE_CURRENT_SOURCE_DIR}/MOAITestMgr.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MOAITest.cpp
)

moai_needs(uslscore)
# moai_needs(expat)
moai_needs(glew)
# moai_needs(contrib)
moai_needs(box2d)
moai_needs(chipmunk)
moai_needs(freetype)
moai_needs(tinyxml)
# moai_needs(jpg)
# moai_needs(png)
#moai_needs(moaicore)
moai_needs(lua-lib)
# moai_needs(curl)
# moai_needs(ssl)
# moai_needs(crypto)
# moai_needs(cares)
# moai_needs(luaext)