cmake_minimum_required ( VERSION 2.8.5 )

project ( expat )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/expat-2.1.0/lib/" )

if ( BUILD_WINDOWS )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_MEMMOVE=1 -DXML_DTD" )
endif (BUILD_WINDOWS)

if ( BUILD_ANDROID ) 
	set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_MEMMOVE=1 " )
endif (BUILD_ANDROID )


set ( EXPAT_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}" )

file ( GLOB_RECURSE EXPAT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.c" )
file ( GLOB_RECURSE EXPAT_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

add_library ( expat STATIC ${EXPAT_SOURCES} ${EXPAT_HEADERS} )
target_include_directories (expat PUBLIC "$<BUILD_INTERFACE:${EXPAT_INCLUDES}>"
      $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/expat>
)
target_compile_definitions (expat PUBLIC "XML_STATIC")
set_target_properties( expat PROPERTIES FOLDER Third-Party )
install(TARGETS expat EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES 
  ${CMAKE_CURRENT_SOURCE_DIR}expat.h
  ${CMAKE_CURRENT_SOURCE_DIR}expat_external.h
  DESTINATION include/expat
)