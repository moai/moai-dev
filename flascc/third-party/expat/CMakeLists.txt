cmake_minimum_required ( VERSION 2.8.5 )

project ( expat )

set ( CMAKE_CURRENT_SOURCE_DIR "../../../3rdparty/expat-2.0.1/lib/" )

set ( EXPAT_INCLUDES "../../3rdparty/expat-2.0.1/lib/" CACHE INTERNAL "doc string" )

set ( EXPAT_SOURCES 

  ${CMAKE_CURRENT_SOURCE_DIR}/xmlparse.c
  ${CMAKE_CURRENT_SOURCE_DIR}/xmlrole.c
  ${CMAKE_CURRENT_SOURCE_DIR}/xmltok.c
  ${CMAKE_CURRENT_SOURCE_DIR}/xmltok_impl.c
  ${CMAKE_CURRENT_SOURCE_DIR}/xmltok_ns.c
    
CACHE INTERNAL "doc string" )