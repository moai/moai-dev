cmake_minimum_required ( VERSION 2.8.11 )
project ( spine )


set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/spine-c/" )
set ( SPINE_INCLUDES "${MOAI_ROOT}/3rdparty/spine-c/include/" )

file ( GLOB_RECURSE SPINE_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/spine/*.c" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/spine/*.h" 
)
file ( GLOB_RECURSE SPINE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/spine/*.h" )

add_library( spine STATIC ${SPINE_SOURCES} ${SPINE_HEADERS} )

target_include_directories ( spine PUBLIC "$<BUILD_INTERFACE:${SPINE_INCLUDES}>")
set_target_properties( spine PROPERTIES FOLDER Third-Party )
install(TARGETS spine EXPORT libmoai ARCHIVE DESTINATION lib)
