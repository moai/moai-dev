cmake_minimum_required ( VERSION 2.8.5 )
project ( sqlite3 )

if ( MOAI_SQLITE3 )

set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_SQL=1")
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_SQL=1")

set ( CMAKE_CURRENT_SOURCE_DIR "../../../3rdparty/sqlite-3.6.16/" )

set ( SQLITE3_INCLUDES "../../3rdparty/sqlite-3.6.16/" CACHE INTERNAL "doc string" )

set ( SQLITE3_SOURCES 

  ${CMAKE_CURRENT_SOURCE_DIR}/sqlite3.c

CACHE INTERNAL "doc string" )

endif ( MOAI_SQLITE3 )
