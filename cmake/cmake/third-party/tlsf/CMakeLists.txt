cmake_minimum_required ( VERSION 2.8.5 )
project ( tlsf )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/tlsf-2.0/" )

set ( TLSF_INCLUDES
  "${MOAI_ROOT}/3rdparty/tlsf-2.0/"
)

file ( GLOB TLSF_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.c" )
file ( GLOB TLSF_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

add_library ( tlsf STATIC ${TLSF_SOURCES} ${TLSF_HEADERS} )
target_include_directories ( tlsf PUBLIC "$<BUILD_INTERFACE:${TLSF_INCLUDES}>" )
set_target_properties( tlsf PROPERTIES FOLDER Third-Party )
 install(TARGETS tlsf EXPORT libmoai ARCHIVE DESTINATION lib)