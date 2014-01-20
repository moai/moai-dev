cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-sim )

if ( BUILD_ANDROID )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -include ${MOAI_ROOT}/src/zl-vfs/zl_replace.h" )
   set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include ${MOAI_ROOT}/src/zl-vfs/zl_replace.h" )
endif ( BUILD_ANDROID )

set ( MOAI_SIM_INCLUDES 
  "${MOAI_ROOT}/src/"
  "${MOAI_ROOT}/src/config-default/"

  )

include_directories ( 
  ${MOAI_SIM_INCLUDES} 
)

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-sim/" )

# Use all cpp files in directory as source code
file ( GLOB MOAI_SIM_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )

include_directories ( ${MOAI_SIM_INCLUDES} )

add_library ( moai-sim STATIC ${MOAI_SIM_SRC} )

target_link_libraries ( moai-sim moai-core moai-util freetype )

get_target_property ( FREETYPE_INCLUDES freetype INCLUDE_DIRECTORIES )

target_include_directories ( moai-sim PUBLIC ${MOAI_SIM_INCLUDES} ${FREETYPE_INCLUDES} )

if (MOAI_BOX2D)
 target_link_libraries ( moai-sim moai-box2d)
endif (MOAI_BOX2D)

if (MOAI_CHIPMUNK)
	target_link_libraries ( moai-sim moai-chipmunk)
endif (MOAI_CHIPMUNK)

if (BUILD_WINDOWS)
 target_link_libraries ( moai-sim  psapi rpcrt4 ) #GetProcessMemoryInfo and UuidCreateSequential
endif (BUILD_WINDOWS)
