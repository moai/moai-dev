cmake_minimum_required ( VERSION 2.8.11 )
project ( moai-ios-mobile-app-tracker )
#native
set(APPTRACKER_ROOT ${MOAI_ROOT}/3rdparty/MobileAppTracking-ios-2.6/sdk-ios/MobileAppTracker.framework)
set(APPTRACKER_LIB ${APPTRACKER_ROOT}/MobileAppTracker)
add_library(MobileAppTracker SHARED IMPORTED)
set_property(TARGET MobileAppTracker PROPERTY IMPORTED_LOCATION ${APPTRACKER_LIB})
set_property(TARGET MobileAppTracker PROPERTY FRAMEWORK 1)

install(DIRECTORY "${APPTRACKER_ROOT}" DESTINATION lib)


#glue
set (CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-ios-mobile-app-tracker")
file ( GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.mm" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )
add_library(moai-ios-mobile-app-tracker STATIC  ${SRC})

target_include_directories(moai-ios-mobile-app-tracker PUBLIC
		"$<BUILD_INTERFACE:${MOAI_ROOT}/src>"
		"$<BUILD_INTERFACE:${APPTRACKER_ROOT}>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
	)

target_link_libraries( moai-ios-mobile-app-tracker moai-sim moai-util moai-core MobileAppTracker)
set_target_properties( moai-ios-mobile-app-tracker PROPERTIES FOLDER Modules)
install(TARGETS moai-ios-mobile-app-tracker EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES ${MOAI_ROOT}/src/moai-ios-mobile-app-tracker/host.h DESTINATION include/moai-ios-mobile-app-tracker)
