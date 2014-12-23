cmake_minimum_required ( VERSION 2.8.5 )
project ( libtess )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/libtess2/" )

set ( MOD_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/Include"  )

file ( GLOB MOD_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.c" )
file ( GLOB MOD_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.h"  "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h")

add_library ( tess STATIC ${MOD_SOURCES} ${MOD_HEADERS} )

target_include_directories ( tess PUBLIC "$<BUILD_INTERFACE:${MOD_INCLUDES}>" )

set_target_properties( tess PROPERTIES FOLDER Third-Party )

install(TARGETS tess EXPORT libmoai ARCHIVE DESTINATION lib)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Include/
        DESTINATION include/libtess2 )