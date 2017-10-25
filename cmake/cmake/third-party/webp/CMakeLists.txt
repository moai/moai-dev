cmake_minimum_required ( VERSION 2.8.5 )
project ( webp )

add_definitions( -DWEBP_SWAP_16BIT_CSP ) # Fix endianness for quantized formats

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/libwebp-0.4.1/src" )

set ( WEBP_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/webp"  )

file ( GLOB_RECURSE WEBP_SOURCE 
		"${WEBP_INCLUDES}/*.h" 
		"${CMAKE_CURRENT_SOURCE_DIR}/dec/*.h" 
		"${CMAKE_CURRENT_SOURCE_DIR}/dec/*.c" 
		"${CMAKE_CURRENT_SOURCE_DIR}/demux/*.h" 
		"${CMAKE_CURRENT_SOURCE_DIR}/demux/*.c" 
		"${CMAKE_CURRENT_SOURCE_DIR}/utils/*.h" 
		"${CMAKE_CURRENT_SOURCE_DIR}/utils/*.c" 
		"${CMAKE_CURRENT_SOURCE_DIR}/dsp/*.h" )

set ( WEBP_DSP_C 
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/alpha_processing.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/cpu.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/dec.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/dec_clip_tables.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/dec_mips32.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/dec_sse2.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/enc.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/enc_avx2.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/enc_mips32.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/enc_sse2.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/lossless.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/lossless_mips32.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/lossless_sse2.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/upsampling.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/upsampling_sse2.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/yuv.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/yuv_mips32.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp/yuv_sse2.c 
)

set (NEON_FILES 
  "${CMAKE_CURRENT_SOURCE_DIR}/dsp/dec_neon.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/dsp/enc_neon.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/dsp/lossless_neon.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/dsp/upsampling_neon.c"
)

if (ARMEABI_V7A) 
  string(REGEX MATCH "-mfpu=[a-zA-Z0-9\\-]*"  OLD_FPU_FLAG ${CMAKE_C_FLAGS})
  string(REGEX REPLACE "-mfpu=[a-zA-Z0-9\\-]*" "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
endif (ARMEABI_V7A)


add_library ( webp STATIC
  ${WEBP_SOURCE} ${WEBP_DSP_C} ${NEON_FILES}
)

if (ARMEABI_V7A)
   set_source_files_properties(${WEBP_SOURCE} ${WEBP_DSP_C} PROPERTIES COMPILE_FLAGS ${OLD_FPU_FLAG})
   set_source_files_properties(${NEON_FILES} PROPERTIES COMPILE_FLAGS "-mfpu=neon")
endif (ARMEABI_V7A)


set_target_properties( webp PROPERTIES FOLDER Third-Party )

if (BUILD_ANDROID)
	target_link_libraries ( webp cpufeatures)
endif ( BUILD_ANDROID )

target_include_directories ( webp PUBLIC 
  "$<BUILD_INTERFACE:${WEBP_INCLUDES}>"
  $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/webp>
)

set_target_properties( webp PROPERTIES FOLDER Third-Party )

install(TARGETS webp EXPORT libmoai ARCHIVE DESTINATION lib)
file( GLOB WEBP_PUBLIC_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/webp/*.h")
install(FILES ${WEBP_PUBLIC_HEADERS} DESTINATION include/webp)
    
 
