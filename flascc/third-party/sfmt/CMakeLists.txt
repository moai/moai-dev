cmake_minimum_required ( VERSION 2.8.5 )
project ( sfmt )


set ( CMAKE_CURRENT_SOURCE_DIR "../../../3rdparty/sfmt-1.4/" )
set ( SFMT_INCLUDES "../../3rdparty/sfmt-1.4/" CACHE INTERNAL "doc string" )

set ( SFMT_MEXP 19937 )
set ( SFMT_SOURCES 

  ${CMAKE_CURRENT_SOURCE_DIR}/SFMT.c

CACHE INTERNAL "doc string" )