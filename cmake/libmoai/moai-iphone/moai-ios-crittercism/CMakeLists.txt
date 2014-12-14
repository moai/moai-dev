cmake_minimum_required ( VERSION 2.8.11 )
project ( moai-ios-crittercism )

set(CRITTERCISM_LIB ${MOAI_ROOT}/3rdparty/crittercism-4.3.3/CrittercismSDK/libCrittercism_v4_3_3.a)
add_library(Crittercism_v4_3_3 STATIC IMPORTED)
set_property(TARGET Crittercism_v4_3_3 PROPERTY IMPORTED_LOCATION ${CRITTERCISM_LIB})


install(FILES ${CRITTERCISM_LIB} DESTINATION lib)



set (CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-ios-crittercism")
file ( GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.mm" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )
add_library(moai-ios-crittercism STATIC  ${SRC})

target_include_directories(moai-ios-crittercism PUBLIC 
		"$<BUILD_INTERFACE:${MOAI_ROOT}/src>"
		"$<BUILD_INTERFACE:${MOAI_ROOT}/3rdparty/crittercism-4.3.3/CrittercismSDK>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
	)

target_link_libraries( moai-ios-crittercism moai-sim moai-util moai-core Crittercism_v4_3_3)
set_target_properties( moai-ios-crittercism PROPERTIES FOLDER Modules)
install(TARGETS moai-ios-crittercism EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES ${MOAI_ROOT}/src/moai-ios-crittercism/host.h DESTINATION include/moai-ios-crittercism)
