cmake_minimum_required ( VERSION 2.8.5 )
project ( png )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/lpng140/" )

set ( PNG_INCLUDES "${MOAI_ROOT}/3rdparty/lpng140/" )

file ( GLOB PNG_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.c" )
file ( GLOB PNG_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )


include_directories (${PNG_INCLUDES})
add_library(png STATIC ${PNG_SOURCES})
target_link_libraries ( png zlib )
target_compile_definitions (png PUBLIC "PNG_SKIP_SETJMP_CHECK=1")
target_include_directories(png PUBLIC "$<BUILD_INTERFACE:${PNG_INCLUDES}>")
set_target_properties( png PROPERTIES FOLDER Third-Party )
 install(TARGETS png EXPORT libmoai ARCHIVE DESTINATION lib)