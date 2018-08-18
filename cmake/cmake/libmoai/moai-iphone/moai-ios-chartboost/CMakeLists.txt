cmake_minimum_required ( VERSION 2.8.11 )
project ( moai-ios-chartboost )

# lib
set(CHARTBOOST_LIB ${MOAI_ROOT}/3rdparty/chartboost-4.2/libChartboost.a)
add_library(Chartboost STATIC IMPORTED)
set_property(TARGET Chartboost PROPERTY IMPORTED_LOCATION ${CHARTBOOST_LIB})
#target_include_directories(libChartboost PUBLIC "${MOAI_ROOT}/3rdparty/chartboost-4.2")

install(FILES ${CHARTBOOST_LIB} DESTINATION lib)

#glue
set (CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-ios-chartboost")
file ( GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.mm" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )
add_library(moai-ios-chartboost STATIC  ${SRC})

target_include_directories(moai-ios-chartboost PUBLIC 
		"$<BUILD_INTERFACE:${MOAI_ROOT}/src>"
		"$<BUILD_INTERFACE:${MOAI_ROOT}/3rdparty/chartboost-4.2>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
	)

target_link_libraries( moai-ios-chartboost moai-sim moai-util moai-core Chartboost)
set_target_properties( moai-ios-chartboost PROPERTIES FOLDER Modules)
install(TARGETS moai-ios-chartboost EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES ${MOAI_ROOT}/src/moai-ios-chartboost/host.h DESTINATION include/moai-ios-chartboost)
