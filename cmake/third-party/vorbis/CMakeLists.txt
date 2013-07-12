cmake_minimum_required ( VERSION 2.8.5 )
project ( vorbis )

if ( MOAI_VORBIS )

  set ( CMAKE_CURRENT_SOURCE_DIR "../../3rdparty/libvorbis-1.3.2/lib" )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_VORBIS=1" CACHE INTERNAL "doc string" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_VORBIS=1" CACHE INTERNAL "doc string" )

  set ( VORBIS_INCLUDES "../../3rdparty/libvorbis-1.3.2/lib/;../../3rdparty/libvorbis-1.3.2/include/" CACHE INTERNAL "doc string" )

  set ( VORBIS_SOURCES 

    ${CMAKE_CURRENT_SOURCE_DIR}/analysis.c
    #${CMAKE_CURRENT_SOURCE_DIR}/barkmel.c
    ${CMAKE_CURRENT_SOURCE_DIR}/bitrate.c
    ${CMAKE_CURRENT_SOURCE_DIR}/block.c
    ${CMAKE_CURRENT_SOURCE_DIR}/codebook.c
    ${CMAKE_CURRENT_SOURCE_DIR}/envelope.c
    ${CMAKE_CURRENT_SOURCE_DIR}/floor0.c
    ${CMAKE_CURRENT_SOURCE_DIR}/floor1.c
    ${CMAKE_CURRENT_SOURCE_DIR}/info.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lookup.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lpc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lsp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mapping0.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mdct.c
    ${CMAKE_CURRENT_SOURCE_DIR}/psy.c
    #${CMAKE_CURRENT_SOURCE_DIR}/psytune.c
    ${CMAKE_CURRENT_SOURCE_DIR}/registry.c
    ${CMAKE_CURRENT_SOURCE_DIR}/res0.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sharedbook.c
    ${CMAKE_CURRENT_SOURCE_DIR}/smallft.c
    ${CMAKE_CURRENT_SOURCE_DIR}/synthesis.c
    #${CMAKE_CURRENT_SOURCE_DIR}/tone.c
    ${CMAKE_CURRENT_SOURCE_DIR}/vorbisenc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/vorbisfile.c
    ${CMAKE_CURRENT_SOURCE_DIR}/window.c

  CACHE INTERNAL "doc string" )

else ( MOAI_VORBIS )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_VORBIS=0" CACHE INTERNAL "doc string" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_VORBIS=0" CACHE INTERNAL "doc string" )

endif ( MOAI_VORBIS )
