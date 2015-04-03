cmake_minimum_required ( VERSION 2.8.11 )
project ( moai-ios-kontagent )

#native
set(KONTAGENT_ROOT ${MOAI_ROOT}/3rdparty/kontagent_ios_v1.5.6/Kontagent.framework)
set(KONTAGENT_LIB ${KONTAGENT_ROOT}/Kontagent)

add_library(Kontagent STATIC IMPORTED)
set_property(TARGET Kontagent PROPERTY IMPORTED_LOCATION ${KONTAGENT_LIB})
set_property(TARGET Kontagent PROPERTY FRAMEWORK 1)

install(DIRECTORY "${KONTAGENT_ROOT}" DESTINATION lib )


#glue
set (CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-ios-kontagent")
file ( GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.mm" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )
add_library(moai-ios-kontagent STATIC  ${SRC})

target_include_directories(moai-ios-kontagent PUBLIC 
		"$<BUILD_INTERFACE:${MOAI_ROOT}/src>"
		"$<BUILD_INTERFACE:${KONTAGENT_ROOT}>"
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
	)

target_link_libraries( moai-ios-kontagent moai-sim moai-util moai-core Kontagent)
set_target_properties( moai-ios-kontagent PROPERTIES FOLDER Modules)
install(TARGETS moai-ios-kontagent EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES ${MOAI_ROOT}/src/moai-ios-kontagent/host.h DESTINATION include/moai-ios-kontagent)
