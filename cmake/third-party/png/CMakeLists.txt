cmake_minimum_required ( VERSION 2.8.5 )
project ( png )



  set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/lpng140/" )

  set ( PNG_INCLUDES "${MOAI_ROOT}/3rdparty/lpng140/" )

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

 )

include_directories (${PNG_INCLUDES})
add_library(png STATIC ${PNG_SOURCES})
target_link_libraries ( png zlib )
target_include_directories(png PUBLIC ${PNG_INCLUDES})