cmake_minimum_required ( VERSION 2.8.11 )
project ( moai-ios-billing )

set (CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-ios-billing")
file ( GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.mm" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )
add_library(moai-ios-billing STATIC  ${SRC})

target_include_directories(moai-ios-billing PUBLIC 
		"$<BUILD_INTERFACE:${MOAI_ROOT}/src>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
	)

target_link_libraries( moai-ios-billing moai-sim moai-util moai-core)
set_target_properties( moai-ios-billing PROPERTIES FOLDER Modules)
install(TARGETS moai-ios-billing EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES ${MOAI_ROOT}/src/moai-ios-billing/host.h DESTINATION include/moai-ios-billing)
