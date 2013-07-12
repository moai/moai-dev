cmake_minimum_required ( VERSION 2.8.5 )
project ( ogg )

if ( MOAI_OGG )

  set ( CMAKE_CURRENT_SOURCE_DIR "../../3rdparty/libogg-1.2.2/src/" )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_OGG=1" CACHE INTERNAL "doc string" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_OGG=1" CACHE INTERNAL "doc string" )

  set ( OGG_INCLUDES "../../3rdparty/libogg-1.2.2/include/" CACHE INTERNAL "doc string" )

  set ( OGG_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/bitwise.c
    ${CMAKE_CURRENT_SOURCE_DIR}/framing.c

  CACHE INTERNAL "doc string" )

else ( MOAI_OGG )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_OGG=0" CACHE INTERNAL "doc string" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_OGG=0" CACHE INTERNAL "doc string" )

endif ( MOAI_OGG )
