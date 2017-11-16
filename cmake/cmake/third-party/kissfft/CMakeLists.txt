cmake_minimum_required ( VERSION 2.8.5 )
project ( kissfft )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/kissfft/" )

set ( MOD_INCLUDES "${MOAI_ROOT}/3rdparty/kissfft/" "${MOAI_ROOT}/3rdparty/"  )

set ( MOD_SOURCES 
  ${CMAKE_CURRENT_SOURCE_DIR}/kiss_fft.c
  ${CMAKE_CURRENT_SOURCE_DIR}/tools/kiss_fftr.c
)

file ( GLOB_RECURSE MOD_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

include_directories ( $MOD_INCLUDES} )
add_library ( kissfft STATIC ${MOD_SOURCES} ${MOD_HEADERS} )
target_include_directories ( kissfft PUBLIC "$<BUILD_INTERFACE:${MOD_INCLUDES}>" )
set_target_properties( kissfft PROPERTIES FOLDER Third-Party )
install(TARGETS kissfft EXPORT libmoai ARCHIVE DESTINATION lib)