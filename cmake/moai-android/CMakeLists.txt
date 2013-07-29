cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-android )

if ( BUILD_ANDROID )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -include ${CMAKE_CURRENT_SOURCE_DIR}/../../src/zl-vfs/zl_replace.h" )
   set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include ${CMAKE_CURRENT_SOURCE_DIR}/../../src/zl-vfs/zl_replace.h" )
endif ( BUILD_ANDROID )

set ( MOAI_ANDROID_INCLUDES 
  ${THIRD_PARTY_INCLUDES}
  "../../src/"
  "../../src/config-default/"

 CACHE INTERNAL "doc string" )

include_directories ( 
  ${MOAI_ANDROID_INCLUDES} 
)

set ( CMAKE_CURRENT_SOURCE_DIR "../../src/moai-android/" )

# Use all cpp files in directory as source code
file ( GLOB MOAI_ANDROID_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )

include_directories ( ${MOAI_ANDROID_INCLUDES} )

add_library ( moai-android STATIC ${MOAI_ANDROID_SRC} )