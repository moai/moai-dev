cmake_minimum_required(VERSION 2.8.5)
project(lua-lib)

# Check to ensure that we are running as part of the
# root CMakeLists.txt file.
if(NOT DEFINED IS_ROOT)
  message(FATAL_ERROR "You must generate the CMake files as part of the root directory.")
endif(NOT DEFINED IS_ROOT)

set(${PROJECT_NAME}_UnixInclude "${ROOT_DIR}/3rdparty/lua-5.1.3/src/" PARENT_SCOPE)
set(CMAKE_CURRENT_SOURCE_DIR "${ROOT_DIR}/3rdparty/lua-5.1.3/src/")
add_definitions(-DLUA_USE_LINUX)

moai_manual(library
    ${CMAKE_CURRENT_SOURCE_DIR}/lapi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lauxlib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lbaselib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lcode.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ldblib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ldebug.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ldo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ldump.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lfunc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lgc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/linit.c
    ${CMAKE_CURRENT_SOURCE_DIR}/liolib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/llex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lmathlib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lmem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/loadlib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lopcodes.c
    ${CMAKE_CURRENT_SOURCE_DIR}/loslib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lparser.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lstate.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lstring.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lstrlib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ltable.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ltablib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ltm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lundump.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lvm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lzio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/print.c
)
moai_needs(m)
target_link_libraries(lua-lib ${CMAKE_DL_LIBS})
