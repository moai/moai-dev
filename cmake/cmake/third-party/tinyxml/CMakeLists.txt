cmake_minimum_required ( VERSION 2.8.5 )
project ( tinyxml )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/tinyxml/" )

set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTIXML_USE_STL"  )

set ( TINYXML_INCLUDES "${MOAI_ROOT}/3rdparty/tinyxml/" )

file ( GLOB TINYXML_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )
file ( GLOB TINYXML_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )
list ( REMOVE_ITEM TINYXML_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/xmltest.cpp")

add_library (tinyxml STATIC ${TINYXML_SOURCES} ${TINYXML_HEADERS} )
target_include_directories ( tinyxml PUBLIC 
	"$<BUILD_INTERFACE:${TINYXML_INCLUDES}>"
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/tinyxml>"
	 )
set_target_properties ( tinyxml PROPERTIES FOLDER Third-Party )
 install(TARGETS tinyxml EXPORT libmoai ARCHIVE DESTINATION lib)
 install(FILES "${MOAI_ROOT}/3rdparty/tinyxml/tinyxml.h" 
 	"${MOAI_ROOT}/3rdparty/tinyxml/tinystr.h" 
 	DESTINATION "include/tinyxml")
 