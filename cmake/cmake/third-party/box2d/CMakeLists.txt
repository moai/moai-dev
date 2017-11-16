cmake_minimum_required ( VERSION 2.8.11 )
project ( box2d )


set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/box2d-2.3.0/" )
set ( BOX2D_INCLUDES "${MOAI_ROOT}/3rdparty/box2d-2.3.0/" )

file ( GLOB_RECURSE BOX2D_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Box2D/*.cpp" )
file ( GLOB_RECURSE BOX2D_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Box2D/*.h" )

add_library( box2d STATIC ${BOX2D_SOURCES} ${BOX2D_HEADERS} )

target_include_directories ( box2d PUBLIC "$<BUILD_INTERFACE:${BOX2D_INCLUDES}>")
set_target_properties( box2d PROPERTIES FOLDER Third-Party )
install(TARGETS box2d EXPORT libmoai ARCHIVE DESTINATION lib)