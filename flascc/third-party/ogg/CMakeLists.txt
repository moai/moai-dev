cmake_minimum_required ( VERSION 2.8.5 )
project ( ogg )

set ( CMAKE_CURRENT_SOURCE_DIR "../../../3rdparty/libogg-1.2.2/src/" )

set ( OGG_INCLUDES "../../3rdparty/libogg-1.2.2/include/" CACHE INTERNAL "doc string" )

set ( OGG_SOURCES 
  ${CMAKE_CURRENT_SOURCE_DIR}/bitwise.c
  ${CMAKE_CURRENT_SOURCE_DIR}/framing.c

CACHE INTERNAL "doc string" )