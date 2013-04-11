cmake_minimum_required ( VERSION 2.8.5 )
project(uslscore)

# Define source directory
set ( CMAKE_CURRENT_SOURCE_DIR "../../src/uslscore/" )

# Define needed include files
set ( USLSCORE_INCLUDES "../../src/;../../3rdparty/;../../src/config-default/;../../3rdparty/ooid-0.99/;" CACHE INTERNAL "doc string" )

# Use all cpp files in directory as source code
file ( GLOB SRC_USLSCORE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )

include_directories ( ${USLSCORE_INCLUDES} ${LUA_INCLUDES} ${SSL_INCLUDES} ${EXPAT_INCLUDES} )

add_library ( uslscore STATIC ${SRC_USLSCORE} )