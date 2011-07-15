find_package(CURL REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLUT REQUIRED)
find_package(Lua51 REQUIRED)
find_package(PNG REQUIRED)
find_package(ZLIB REQUIRED)

include_directories(
    ${MOAI_SOURCE_DIR}/3rdparty
    ${MOAI_SOURCE_DIR}/3rdparty/box2d-2.1.2/Box2D
    ${MOAI_SOURCE_DIR}/3rdparty/chipmunk-5.3.4/include
    ${MOAI_SOURCE_DIR}/3rdparty/expat-2.0.1/lib
    ${MOAI_SOURCE_DIR}/3rdparty/glew-1.5.6/include
    # Private Lua API is used, requiring inclusion of private header lstate.h
    ${MOAI_SOURCE_DIR}/3rdparty/lua-5.1.3/src
    ${MOAI_SOURCE_DIR}/3rdparty/sqlite-3.6.16
    ${MOAI_SOURCE_DIR}/3rdparty/tinyxml
    ${MOAI_SOURCE_DIR}/src
    ${CURL_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIRS}
    ${GLUT_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${PNG_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIRS}
    )

link_directories(
    ${MOAI_SOURCE_DIR}/3rdparty/box2d-2.1.2/Box2D/Box2D
    ${MOAI_SOURCE_DIR}/3rdparty/chipmunk-5.3.4/src
    ${MOAI_SOURCE_DIR}/3rdparty/contrib
    ${MOAI_SOURCE_DIR}/3rdparty/sqlite-3.6.16
    ${MOAI_SOURCE_DIR}/3rdparty/tinyxml
    )

add_definitions(
    ${PNG_DEFINITIONS}
    )

add_subdirectory(aku)
add_subdirectory(moai)
add_subdirectory(moaicore)
add_subdirectory(uslscore)
add_subdirectory(uslsext)
