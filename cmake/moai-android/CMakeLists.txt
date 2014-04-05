cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-android )

if ( BUILD_ANDROID )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -include ${MOAI_ROOT}/src/zl-vfs/zl_replace.h" )
   set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include ${MOAI_ROOT}/src/zl-vfs/zl_replace.h" )
endif ( BUILD_ANDROID )

set ( MOAI_ANDROID_INCLUDES 
  "${MOAI_ROOT}/src/"
  "${MOAI_ROOT}/src/config-default/"
 )

include_directories ( 
  ${MOAI_ANDROID_INCLUDES} 
)

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-android/" )

# Use all cpp files in directory as source code
file ( GLOB MOAI_ANDROID_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

add_library ( moai-android STATIC ${MOAI_ANDROID_SRC} )
target_link_libraries( moai-android moai-sim moai-util moai-core )