cmake_minimum_required ( VERSION 2.8.5 )
project ( sfmt )

if ( MOAI_SFMT )

  set ( CMAKE_CURRENT_SOURCE_DIR "../../3rdparty/sfmt-1.4/" )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_SFMT=1 -DSFMT_MEXP=19937" CACHE INTERNAL "doc string" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_SFMT=1 -DSFMT_MEXP=19937" CACHE INTERNAL "doc string" )

  set ( SFMT_INCLUDES "../../3rdparty/sfmt-1.4/" CACHE INTERNAL "doc string" )

  set ( SFMT_MEXP 19937 )
  set ( SFMT_SOURCES 

    ${CMAKE_CURRENT_SOURCE_DIR}/SFMT.c

  CACHE INTERNAL "doc string" )

else ( MOAI_SFMT )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_SFMT=0" CACHE INTERNAL "doc string" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_SFMT=0" CACHE INTERNAL "doc string" )

endif ( MOAI_SFMT )
