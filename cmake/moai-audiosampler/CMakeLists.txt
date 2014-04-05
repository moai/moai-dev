cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-audiosampler )

set ( MOAI_AUDIOSAMPLER_INCLUDES
  "${MOAI_ROOT}/src/"
  "${MOAI_ROOT}/src/config-default/"
  )

include_directories (
  ${MOAI_AUDIOSAMPLER_INCLUDES}
)

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-audiosampler/" )

# Use all cpp files in directory as source code
file ( GLOB MOAI_AUDIOSAMPLER_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

add_library ( moai-audiosampler STATIC ${MOAI_AUDIOSAMPLER_SRC}  )
target_link_libraries ( moai-audiosampler moai-core moai-util moai-sim zlcore )
set_target_properties ( moai-audiosampler PROPERTIES FOLDER Modules )  

