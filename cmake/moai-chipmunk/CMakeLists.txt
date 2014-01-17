cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-chipmunk )

if ( MOAI_CHIPMUNK )
set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-chipmunk/" )
set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DAKU_WITH_CHIPMUNK=1"  PARENT_SCOPE )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAKU_WITH_CHIPMUNK=1"  PARENT_SCOPE )

set ( MOAI_CHIPMUNK_INCLUDES 
  "${MOAI_ROOT}/src/"
  "${MOAI_ROOT}/src/config-default/"
 )

# Use all cpp files in directory as source code
file ( GLOB MOAI_CHIPMUNK_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )

include_directories ( ${MOAI_CHIPMUNK_INCLUDES} )

add_library ( moai-chipmunk STATIC ${MOAI_CHIPMUNK_SRC} )

target_link_libraries ( moai-chipmunk third-party)

else ( MOAI_CHIPMUNK )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DAKU_WITH_CHIPMUNK=0" PARENT_SCOPE )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAKU_WITH_CHIPMUNK=0"  PARENT_SCOPE )
endif ( MOAI_CHIPMUNK )
