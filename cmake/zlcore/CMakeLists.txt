cmake_minimum_required ( VERSION 2.8.5 )
project ( zlcore )

# Define source directory
set ( CMAKE_CURRENT_SOURCE_DIR "../../src/" )

# Define needed include files
set ( ZLCORE_INCLUDES 
  ${THIRD_PARTY_INCLUDES}
  "../../src/"
  "../../src/config-default/"
CACHE INTERNAL "doc string" )

# Use all cpp files in directory as source code
file ( GLOB SRC_ZLCORE "${CMAKE_CURRENT_SOURCE_DIR}/zl-*/*.cpp" )

include_directories ( ${ZLCORE_INCLUDES} )

add_library ( zlcore STATIC ${SRC_ZLCORE} )

if (BUILD_WINDOWS)
  target_link_libraries ( zlcore iphlpapi) #GetAdaptersAddresses
endif (BUILD_WINDOWS)