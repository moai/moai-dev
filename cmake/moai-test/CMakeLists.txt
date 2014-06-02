cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-test )

set ( MOAI_TEST_INCLUDES 
  "${MOAI_ROOT}/src/"
  "${MOAI_ROOT}/src/config-default/"
 )

include_directories ( 
  ${MOAI_CORE_INCLUDES} 
)

# Use all cpp files in directory as source code
set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-test/" )
file ( GLOB MOAI_TEST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

include_directories ( ${MOAI_TEST_INCLUDES} )

add_library ( moai-test STATIC ${MOAI_TEST_SRC} )

if (NOT MOAI_TINYXML)
	message(FATAL_ERROR "MOAI_TINYXML must be enabled for moai-test")
endif (NOT MOAI_TINYXML)

target_link_libraries ( moai-test moai-sim moai-util moai-core zlcore tinyxml)
set_target_properties(moai-test PROPERTIES FOLDER Modules)  