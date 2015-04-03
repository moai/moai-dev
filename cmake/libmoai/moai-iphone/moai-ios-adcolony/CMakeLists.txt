cmake_minimum_required ( VERSION 2.8.11 )
project ( moai-ios-adcolony )



set(ADCOLONY_ROOT ${MOAI_ROOT}/3rdparty/adcolony-2.2.4/AdColony.framework)
set(ADCOLONY_LIB ${ADCOLONY_ROOT}/AdColony)
add_library(AdColony SHARED IMPORTED)
set_property(TARGET AdColony PROPERTY IMPORTED_LOCATION ${ADCOLONY_LIB})
set_property(TARGET AdColony PROPERTY FRAMEWORK 1)

install(DIRECTORY "${ADCOLONY_ROOT}" DESTINATION lib)


add_library(moai-ios-adcolony STATIC
		"${MOAI_ROOT}/src/moai-ios-adcolony/MOAIAdColonyIOS.h"
		"${MOAI_ROOT}/src/moai-ios-adcolony/MOAIAdColonyIOS.mm"
		"${MOAI_ROOT}/src/moai-ios-adcolony/host.mm"
		)
target_include_directories(moai-ios-adcolony PUBLIC
		"$<BUILD_INTERFACE:${MOAI_ROOT}/src>"
		"$<BUILD_INTERFACE:${ADCOLONY_ROOT}>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
	)

target_link_libraries(moai-ios-adcolony AdColony moai-sim moai-util moai-core)
set_target_properties( moai-ios-adcolony PROPERTIES FOLDER Modules)
install(TARGETS moai-ios-adcolony EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES ${MOAI_ROOT}/src/moai-ios-adcolony/host.h DESTINATION include/moai-ios-adcolony)
