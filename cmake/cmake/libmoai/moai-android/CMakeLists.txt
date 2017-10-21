cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-android )

set ( MOAI_ANDROID_INCLUDES 
"${MOAI_ROOT}/src/"
"${CMAKE_BINARY_DIR}/libmoai/config"
)


set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-android" )

file ( GLOB MOAI_ANDROID_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )


add_library (moai-android STATIC ${MOAI_ANDROID_SRC} )
target_link_libraries( moai-android moai-sim moai-util moai-core )
target_include_directories ( moai-android PUBLIC ${MOAI_ANDROID_INCLUDES})

install(TARGETS moai-android EXPORT libmoai ARCHIVE DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION include/moai-android
          FILES_MATCHING PATTERN "*.h")


