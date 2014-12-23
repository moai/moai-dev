cmake_minimum_required ( VERSION 2.8.11 )
project ( moai-ios-flurry )

# lib
set(FLURRY_ROOT ${MOAI_ROOT}/3rdparty/Flurry-iOS-5.0.0/Flurry)
set(FLURRY_LIB ${FLURRY_ROOT}/libFlurry_5.0.0.a)

add_library(Flurry STATIC IMPORTED)
set_property(TARGET Flurry PROPERTY IMPORTED_LOCATION ${FLURRY_LIB})

install(FILES ${FLURRY_LIB} DESTINATION lib RENAME libFlurry.a)

#glue
set (CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-ios-flurry")
file ( GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.mm" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )
add_library(moai-ios-flurry STATIC  ${SRC})

target_include_directories(moai-ios-flurry PUBLIC 
		"$<BUILD_INTERFACE:${MOAI_ROOT}/src>"
		"$<BUILD_INTERFACE:${FLURRY_ROOT}>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
	)

target_link_libraries( moai-ios-flurry moai-sim moai-util moai-core Flurry)
set_target_properties( moai-ios-flurry PROPERTIES FOLDER Modules)
install(TARGETS moai-ios-flurry EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES ${MOAI_ROOT}/src/moai-ios-flurry/host.h DESTINATION include/moai-ios-flurry)
