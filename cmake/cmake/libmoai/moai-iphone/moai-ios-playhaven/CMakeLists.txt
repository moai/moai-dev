cmake_minimum_required ( VERSION 2.8.11 )
project ( moai-ios-playhaven )


add_subdirectory(playhaven)

set (CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-ios-playhaven")
file ( GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.mm" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )
add_library(moai-ios-playhaven STATIC  ${SRC})

target_include_directories(moai-ios-playhaven PUBLIC 
		"$<BUILD_INTERFACE:${MOAI_ROOT}/src>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
	)

target_link_libraries( moai-ios-playhaven moai-sim moai-util moai-core playhaven-ios)
set_target_properties( moai-ios-playhaven PROPERTIES FOLDER Modules)
install(TARGETS moai-ios-playhaven EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES ${MOAI_ROOT}/src/moai-ios-playhaven/host.h DESTINATION include/moai-ios-playhaven)
