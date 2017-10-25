cmake_minimum_required ( VERSION 2.8.5 )

project ( contrib )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/contrib/" )

set ( CONTRIB_INCLUDES
  "${MOAI_ROOT}/3rdparty/"
)

file ( GLOB_RECURSE CONTRIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.c" )
file ( GLOB_RECURSE CONTRIB_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

include_directories(${CONTRIB_INCLUDES})
add_library( contrib STATIC ${CONTRIB_SOURCES})
target_include_directories (contrib PUBLIC 
	"$<BUILD_INTERFACE:${CONTRIB_INCLUDES}>"
	"$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/contrib>")
set_target_properties( contrib PROPERTIES FOLDER Third-Party )
install(TARGETS contrib EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES ${CONTRIB_HEADERS} DESTINATION "include/contrib")
