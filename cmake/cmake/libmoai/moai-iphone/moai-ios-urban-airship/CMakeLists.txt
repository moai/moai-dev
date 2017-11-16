cmake_minimum_required ( VERSION 2.8.11 )
project ( moai-ios-urban-airship )

# lib
set(UA_ROOT ${MOAI_ROOT}/3rdparty/libUAirship-3.0.1/Airship)
set(UA_LIB ${UA_ROOT}/libUAirship-3.0.1.a)

add_library(UAirship STATIC IMPORTED)
set_property(TARGET UAirship PROPERTY IMPORTED_LOCATION ${UA_LIB})

install(FILES ${UA_LIB} DESTINATION lib RENAME libUAirship.a)

#glue
set (CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-ios-urban-airship")
file ( GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.mm" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )
add_library(moai-ios-urban-airship STATIC  ${SRC})
target_include_directories(moai-ios-urban-airship PUBLIC 
		"$<BUILD_INTERFACE:${MOAI_ROOT}/src>"
		"$<BUILD_INTERFACE:${UA_ROOT}>"
		"$<BUILD_INTERFACE:${UA_ROOT}/Common>"
		"$<BUILD_INTERFACE:${UA_ROOT}/External>"
		"$<BUILD_INTERFACE:${UA_ROOT}/Inbox>"
		"$<BUILD_INTERFACE:${UA_ROOT}/Push>"
		"$<BUILD_INTERFACE:${UA_ROOT}/UI>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
	)

target_link_libraries( moai-ios-urban-airship moai-sim moai-util moai-core UAirship)
set_target_properties( moai-ios-urban-airship PROPERTIES FOLDER Modules)
install(TARGETS moai-ios-urban-airship EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES ${MOAI_ROOT}/src/moai-ios-urban-airship/host.h DESTINATION include/moai-ios-urban-airship)
