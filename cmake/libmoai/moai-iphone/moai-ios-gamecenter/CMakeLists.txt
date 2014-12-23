cmake_minimum_required ( VERSION 2.8.11 )
project ( moai-ios-gamecenter )

set (CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-ios-gamecenter")
file ( GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.mm" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )
add_library(moai-ios-gamecenter STATIC  ${SRC})

target_include_directories(moai-ios-gamecenter PUBLIC 
		"$<BUILD_INTERFACE:${MOAI_ROOT}/src>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
	)

target_link_libraries( moai-ios-gamecenter moai-sim moai-util moai-core)
set_target_properties( moai-ios-gamecenter PROPERTIES FOLDER Modules)
install(TARGETS moai-ios-gamecenter EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES ${MOAI_ROOT}/src/moai-ios-gamecenter/host.h DESTINATION include/moai-ios-gamecenter)
