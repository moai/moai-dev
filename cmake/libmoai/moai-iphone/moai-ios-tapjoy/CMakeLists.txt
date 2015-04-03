cmake_minimum_required ( VERSION 2.8.11 )
project ( moai-ios-tapjoy )

#native
set(TAPJOY_ROOT ${MOAI_ROOT}/3rdparty/TapjoySDK_iOS_v10.0.2/Libraries/Tapjoy.embeddedframework/Tapjoy.framework)
set(TAPJOY_LIB ${TAPJOY_ROOT}/Tapjoy)

add_library(Tapjoy STATIC IMPORTED)
set_property(TARGET Tapjoy PROPERTY IMPORTED_LOCATION ${TAPJOY_LIB})
set_property(TARGET Tapjoy PROPERTY FRAMEWORK 1)

install(DIRECTORY "${TAPJOY_ROOT}" DESTINATION lib )


#glue
set (CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-ios-tapjoy")
file ( GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.mm" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )
add_library(moai-ios-tapjoy STATIC  ${SRC})

target_include_directories(moai-ios-tapjoy PUBLIC 
		"$<BUILD_INTERFACE:${MOAI_ROOT}/src>"
		"$<BUILD_INTERFACE:${TAPJOY_ROOT}>"
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
	)

target_link_libraries( moai-ios-tapjoy moai-sim moai-util moai-core Tapjoy)
set_target_properties( moai-ios-tapjoy PROPERTIES FOLDER Modules)
install(TARGETS moai-ios-tapjoy EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES ${MOAI_ROOT}/src/moai-ios-tapjoy/host.h DESTINATION include/moai-ios-tapjoy)
