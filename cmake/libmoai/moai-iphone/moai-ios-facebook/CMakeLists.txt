cmake_minimum_required ( VERSION 2.8.11 )
project ( moai-ios-facebook )

#lib
set(FACEBOOK_ROOT ${MOAI_ROOT}/3rdparty/facebook-ios-sdk-3.14.1/FacebookSDK.framework)
set(FACEBOOK_LIB ${FACEBOOK_ROOT}/FacebookSDK)
add_library(FacebookSDK SHARED IMPORTED)
set_property(TARGET FacebookSDK PROPERTY IMPORTED_LOCATION ${FACEBOOK_LIB})
set_property(TARGET FacebookSDK PROPERTY FRAMEWORK 1)

install(DIRECTORY "${FACEBOOK_ROOT}" DESTINATION lib)

#glue
set (CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-ios-facebook")
file ( GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.mm" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )


add_library(moai-ios-facebook STATIC  ${SRC})

target_include_directories(moai-ios-facebook PUBLIC
		"$<BUILD_INTERFACE:${MOAI_ROOT}/src>"
		"$<BUILD_INTERFACE:${FACEBOOK_ROOT}>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
	)

target_link_libraries( moai-ios-facebook moai-sim moai-util moai-core FacebookSDK sqlite3)
set_target_properties( moai-ios-facebook PROPERTIES FOLDER Modules)
install(TARGETS moai-ios-facebook EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES ${MOAI_ROOT}/src/moai-ios-facebook/host.h DESTINATION include/moai-ios-facebook)
