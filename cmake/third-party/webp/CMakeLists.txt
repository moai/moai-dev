cmake_minimum_required ( VERSION 2.8.5 )
project ( webp )

add_definitions( -DWEBP_SWAP_16BIT_CSP ) # Fix endianness for quantized formats

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/libwebp-0.4.1/src" )

set ( WEBP_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/webp"  )

file ( GLOB_RECURSE WEBP_HEADERS "${WEBP_INCLUDES}/*.h" )
file ( GLOB_RECURSE WEBP_DEC_H "${CMAKE_CURRENT_SOURCE_DIR}/dec/*.h" )
file ( GLOB_RECURSE WEBP_DEC_C "${CMAKE_CURRENT_SOURCE_DIR}/dec/*.c" )
file ( GLOB_RECURSE WEBP_DEMUX_H "${CMAKE_CURRENT_SOURCE_DIR}/demux/*.h" )
file ( GLOB_RECURSE WEBP_DEMUX_C "${CMAKE_CURRENT_SOURCE_DIR}/demux/*.c" )
file ( GLOB_RECURSE WEBP_UTILS_H "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.h" )
file ( GLOB_RECURSE WEBP_UTILS_C "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.c" )
file ( GLOB_RECURSE WEBP_DSP_H "${CMAKE_CURRENT_SOURCE_DIR}/dsp/*.h" )
file ( GLOB_RECURSE WEBP_DSP_C "${CMAKE_CURRENT_SOURCE_DIR}/dsp/*.c" )

include_directories ( ${WEBP_INCLUDES} )
add_library ( webp STATIC
  ${WEBP_HEADERS}
  ${WEBP_DEC_H} ${WEBP_DEC_C}
  ${WEBP_DEMUX_H} ${WEBP_DEMUX_C}
  ${WEBP_UTILS_H} ${WEBP_UTILS_C}
  ${WEBP_DSP_H} ${WEBP_DSP_C}
)
target_include_directories ( webp PUBLIC ${WEBP_INCLUDES} )
set_target_properties( webp PROPERTIES FOLDER Third-Party )