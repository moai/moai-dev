cmake_minimum_required ( VERSION 2.8.5 )
project ( png )

if ( MOAI_PNG )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_LIBPNG=1" CACHE INTERNAL "doc string" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_LIBPNG=1" CACHE INTERNAL "doc string" )

  set ( CMAKE_CURRENT_SOURCE_DIR "../../3rdparty/lpng140/" )

  set ( PNG_INCLUDES "../../3rdparty/lpng140/" CACHE INTERNAL "doc string" )

  set ( PNG_SOURCES 

    ${CMAKE_CURRENT_SOURCE_DIR}/png.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngerror.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngget.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngmem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngpread.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngread.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngrio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngrtran.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngrutil.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngset.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngtrans.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngwio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngwrite.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngwtran.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngwutil.c

  CACHE INTERNAL "doc string" )

else ( MOAI_PNG )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_LIBPNG=0" CACHE INTERNAL "doc string" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_LIBPNG=0" CACHE INTERNAL "doc string" )

endif ( MOAI_PNG )
