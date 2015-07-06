cmake_minimum_required ( VERSION 2.8.11 )
project ( moai-ios-twitter )

set (CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-ios-twitter")
file ( GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.mm" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )
add_library(moai-ios-twitter STATIC  ${SRC})

target_include_directories(moai-ios-twitter PUBLIC 
		"$<BUILD_INTERFACE:${MOAI_ROOT}/src>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
	)

target_link_libraries( moai-ios-twitter moai-sim moai-util moai-core moai-apple)
set_target_properties( moai-ios-twitter PROPERTIES FOLDER Modules)
install(TARGETS moai-ios-twitter EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES ${MOAI_ROOT}/src/moai-ios-twitter/host.h DESTINATION include/moai-ios-twitter)
