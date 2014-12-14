cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-audiosampler )

set ( MOAI_AUDIOSAMPLER_INCLUDES
  "${MOAI_ROOT}/src/"
  "${CMAKE_BINARY_DIR}/libmoai/config"
  )

include_directories (
  ${MOAI_AUDIOSAMPLER_INCLUDES}
)

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-audio-sampler/" )

# Use all cpp files in directory as source code
file ( GLOB MOAI_AUDIOSAMPLER_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

if (BUILD_IOS OR BUILD_OSX)
  file (GLOB MOAI_AUDIOSAMPLER_MAC "${CMAKE_CURRENT_SOURCE_DIR}/*.mm" "${CMAKE_CURRENT_SOURCE_DIR}/*.m")
endif (BUILD_IOS OR BUILD_OSX)


add_library ( moai-audiosampler STATIC ${MOAI_AUDIOSAMPLER_SRC} ${MOAI_AUDIOSAMPLER_MAC} )

target_link_libraries ( moai-audiosampler moai-core moai-util moai-sim zlcore )
set_target_properties ( moai-audiosampler PROPERTIES FOLDER Modules )  
target_include_directories ( moai-audiosampler PUBLIC
   "$<BUILD_INTERFACE:${MOAI_AUDIOSAMPLER_INCLUDES}>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
)
install(TARGETS moai-audiosampler EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/host.h DESTINATION include/moai-audio-sampler)
