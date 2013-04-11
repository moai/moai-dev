cmake_minimum_required ( VERSION 2.8.5 )
project ( zlcore )

# Define source directory
set ( CMAKE_CURRENT_SOURCE_DIR "../../src/zlcore/" )

# Define needed include files
set ( ZLCORE_INCLUDES "../../src/;../../3rdparty/tlsf-2.0/" CACHE INTERNAL "doc string" )

# Use all cpp files in directory as source code
file ( GLOB SRC_ZLCORE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )

include_directories ( ${ZLCORE_INCLUDES} )

add_library ( zlcore STATIC ${SRC_ZLCORE} )