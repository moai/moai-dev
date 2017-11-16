cmake_minimum_required ( VERSION 2.8.5 )
project ( mbedtls )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/mbedtls/" )
set ( CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -DMBEDTLS_EXPORTS"  )
set ( TLS_INCLUDES
  "${MOAI_ROOT}/3rdparty/mbedtls/include"
)

file ( GLOB TLS_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/library/*.c" )
file ( GLOB TLS_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h" )

add_library ( mbedtls STATIC ${TLS_SOURCES} ${TLS_HEADERS} )
target_include_directories ( mbedtls PUBLIC "$<BUILD_INTERFACE:${TLS_INCLUDES}>" )
set_target_properties( mbedtls  PROPERTIES FOLDER Third-Party )
 install(TARGETS mbedtls  EXPORT libmoai ARCHIVE DESTINATION lib)