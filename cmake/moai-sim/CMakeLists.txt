cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-sim )

set ( MOAI_SIM_INCLUDES 
  ${THIRD_PARTY_INCLUDES}
  "../../src/"
  "../../src/config-default/"

 CACHE INTERNAL "doc string" )

include_directories ( 
  ${MOAI_SIM_INCLUDES} 
)

set ( CMAKE_CURRENT_SOURCE_DIR "../../src/moai-sim/" )

# Use all cpp files in directory as source code
file ( GLOB MOAI_SIM_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )

include_directories ( ${MOAI_SIM_INCLUDES} )

add_library ( moai-sim STATIC ${MOAI_SIM_SRC} )

if (BUILD_WINDOWS)
 target_link_libraries ( moai-sim  psapi rpcrt4 ) #GetProcessMemoryInfo and UuidCreateSequential
endif (BUILD_WINDOWS)
