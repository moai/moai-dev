cmake_minimum_required ( VERSION 2.8.5 )
project ( vorbis )


set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/libvorbis-1.3.2/lib" )


set ( VORBIS_INCLUDES "${MOAI_ROOT}/3rdparty/libvorbis-1.3.2/lib/;${MOAI_ROOT}/3rdparty/libvorbis-1.3.2/include/"  )

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
)

include_directories (${VORBIS_INCLUDES})
add_library(vorbis STATIC ${VORBIS_SOURCES})
target_include_directories(vorbis PUBLIC "$<BUILD_INTERFACE:${VORBIS_INCLUDES}>")
target_link_libraries( vorbis ogg )
set_target_properties( vorbis PROPERTIES FOLDER Third-Party )
install(TARGETS vorbis EXPORT libmoai ARCHIVE DESTINATION lib)

