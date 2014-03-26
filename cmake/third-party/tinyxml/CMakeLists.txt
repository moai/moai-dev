cmake_minimum_required ( VERSION 2.8.5 )
project ( tinyxml )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/tinyxml/" )

set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTIXML_USE_STL"  )

set ( TINYXML_INCLUDES "${MOAI_ROOT}/3rdparty/tinyxml/" )

file ( GLOB TINYXML_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )
file ( GLOB TINYXML_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )
list ( REMOVE_ITEM TINYXML_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/xmltest.cpp")

add_library (tinyxml STATIC ${TINYXML_SOURCES} ${TINYXML_HEADERS} )
target_include_directories ( tinyxml PUBLIC ${TINYXML_INCLUDES} )
set_target_properties ( tinyxml PROPERTIES FOLDER Third-Party )