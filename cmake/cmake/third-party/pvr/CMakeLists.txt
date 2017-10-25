cmake_minimum_required ( VERSION 2.8.5 )
project ( pvr )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/libpvr-3.4/" )

set ( PVR_INCLUDES "${MOAI_ROOT}/3rdparty/libpvr-3.4/"  )

file ( GLOB_RECURSE PVR_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )
file ( GLOB_RECURSE PVR_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

include_directories ( ${PVR_INCLUDES} )
add_library ( pvr STATIC ${PVR_SOURCES} ${PVR_HEADERS} )
target_include_directories ( pvr PUBLIC "$<BUILD_INTERFACE:${PVR_INCLUDES}>" )
set_target_properties( pvr PROPERTIES FOLDER Third-Party )
install(TARGETS pvr EXPORT libmoai ARCHIVE DESTINATION lib)