cmake_minimum_required ( VERSION 2.8.5 )

project ( contrib )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/contrib/" )

set ( CONTRIB_INCLUDES
  "${MOAI_ROOT}/3rdparty/"
CACHE INTERNAL "doc string" )

set ( CONTRIB_SOURCES

    ${CMAKE_CURRENT_SOURCE_DIR}/utf8.c
    ${CMAKE_CURRENT_SOURCE_DIR}/whirlpool.c

CACHE INTERNAL "doc string" )