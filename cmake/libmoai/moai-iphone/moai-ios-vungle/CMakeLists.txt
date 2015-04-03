cmake_minimum_required ( VERSION 2.8.11 )
project ( moai-ios-vungle )

#native
set(VUNGLE_ROOT ${MOAI_ROOT}/3rdparty/vungle-2.0.1/vunglepub.embeddedframework/vunglepub.framework)
set(VUNGLE_LIB ${VUNGLE_ROOT}/vunglepub)
add_library(vunglepub STATIC IMPORTED)
set_property(TARGET vunglepub PROPERTY IMPORTED_LOCATION ${VUNGLE_LIB})
set_property(TARGET vunglepub PROPERTY FRAMEWORK 1)

install(DIRECTORY ${VUNGLE_ROOT} DESTINATION lib)

#glue
set (CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-ios-vungle")
file ( GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.mm" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )
add_library(moai-ios-vungle STATIC  ${SRC})

target_include_directories(moai-ios-vungle PUBLIC 
		"$<BUILD_INTERFACE:${MOAI_ROOT}/src>"
		"$<BUILD_INTERFACE:${VUNGLE_ROOT}>"
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/lib/vunglepub.framework>"
	)

target_link_libraries( moai-ios-vungle moai-sim moai-util moai-core vunglepub)

set_target_properties( moai-ios-vungle PROPERTIES FOLDER Modules)
install(TARGETS moai-ios-vungle EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES ${MOAI_ROOT}/src/moai-ios-vungle/host.h DESTINATION include/moai-ios-vungle)


