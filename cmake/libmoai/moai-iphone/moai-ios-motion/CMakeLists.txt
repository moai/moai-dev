cmake_minimum_required ( VERSION 2.8.11 )
project ( moai-ios-motion )




set (CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-ios-motion")
file ( GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.mm" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )
add_library(moai-ios-motion STATIC  ${SRC})

target_include_directories(moai-ios-motion PUBLIC 
		"$<BUILD_INTERFACE:${MOAI_ROOT}/src>"
	  "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
	)

target_link_libraries( moai-ios-motion moai-sim moai-util moai-core)
set_target_properties( moai-ios-motion PROPERTIES FOLDER Modules)
install(TARGETS moai-ios-motion EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES ${MOAI_ROOT}/src/moai-ios-motion/host.h DESTINATION include/moai-ios-motion)
