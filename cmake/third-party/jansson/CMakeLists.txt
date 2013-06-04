cmake_minimum_required ( VERSION 2.8.5 )
project ( jansson )

if ( MOAI_JSON )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_JANSSON=1" CACHE INTERNAL "doc string")
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_JANSSON=1" CACHE INTERNAL "doc string")

  set ( CMAKE_CURRENT_SOURCE_DIR "../../3rdparty/jansson-2.1/src/" )

  set ( JANSSON_INCLUDES "../../3rdparty/jansson-2.1/src/" CACHE INTERNAL "doc string" )

  set ( JANSSON_SOURCES 

      ${CMAKE_CURRENT_SOURCE_DIR}/dump.c
      ${CMAKE_CURRENT_SOURCE_DIR}/error.c
      ${CMAKE_CURRENT_SOURCE_DIR}/hashtable.c
      ${CMAKE_CURRENT_SOURCE_DIR}/load.c
      ${CMAKE_CURRENT_SOURCE_DIR}/memory.c
      ${CMAKE_CURRENT_SOURCE_DIR}/pack_unpack.c
      ${CMAKE_CURRENT_SOURCE_DIR}/strbuffer.c
      ${CMAKE_CURRENT_SOURCE_DIR}/utf.c
      ${CMAKE_CURRENT_SOURCE_DIR}/value.c
      
  CACHE INTERNAL "doc string" )

else ( MOAI_JSON )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_JANSSON=0" CACHE INTERNAL "doc string")
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_JANSSON=0" CACHE INTERNAL "doc string")

endif ( MOAI_JSON )
