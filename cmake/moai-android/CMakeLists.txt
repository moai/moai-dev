cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-android )

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