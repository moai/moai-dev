cmake_minimum_required ( VERSION 2.8.5 )
project ( jansson )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/jansson-2.1/src/" )
set ( JANSSON_INCLUDES "${MOAI_ROOT}/3rdparty/jansson-2.1/src/" )

if (BUILD_WINDOWS AND MSVC)
  #need a platform header for this. snprintf doesn't exist
  set ( JANSSON_INCLUDES "${MOAI_ROOT}/3rdparty/jansson-2.1/src-win32" ${JANSSON_INCLUDES})
endif( BUILD_WINDOWS AND MSVC)

file ( GLOB_RECURSE JANSSON_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.c" )
file ( GLOB_RECURSE JANSSON_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

add_library(jansson STATIC ${JANSSON_SOURCES} ${JANSSON_HEADERS} )
target_include_directories(jansson PUBLIC "$<BUILD_INTERFACE:${JANSSON_INCLUDES}>")
set_target_properties( jansson PROPERTIES FOLDER Third-Party )
install(TARGETS jansson EXPORT libmoai ARCHIVE DESTINATION lib)