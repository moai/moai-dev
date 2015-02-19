cmake_minimum_required ( VERSION 2.8.11 )
project ( moai-ios-movie )




set (CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-ios-movie")
file ( GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.mm" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )
add_library(moai-ios-movie STATIC  ${SRC})

target_include_directories(moai-ios-movie PUBLIC 
		"$<BUILD_INTERFACE:${MOAI_ROOT}/src>"
	  "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
	)

target_link_libraries( moai-ios-movie moai-sim moai-util moai-core)
set_target_properties( moai-ios-movie PROPERTIES FOLDER Modules)
install(TARGETS moai-ios-movie EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES ${MOAI_ROOT}/src/moai-ios-movie/host.h DESTINATION include/moai-ios-movie)
