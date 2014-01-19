cmake_minimum_required ( VERSION 2.8.5 )
project ( zlcore )

add_subdirectory ( zlvfs )

# Define source directory
set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/" )

# Define needed include files
set ( ZLCORE_INCLUDES
 "${MOAI_ROOT}/src/config-default/" 
  "${MOAI_ROOT}/src/"
  "${MOAI_ROOT}/3rdparty/"
  "${MOAI_ROOT}/3rdparty/ooid-0.99/"
 )

# Use all cpp files in directory as source code
file ( GLOB SRC_ZLUTIL "${CMAKE_CURRENT_SOURCE_DIR}/zl-util/*.cpp" )
file ( GLOB SRC_ZLGFX "${CMAKE_CURRENT_SOURCE_DIR}/zl-gfx/*.cpp" )

include_directories ( ${ZLCORE_INCLUDES} )

add_library ( zlcore STATIC ${SRC_ZLUTIL} ${SRC_ZLGFX} )

target_include_directories ( zlcore PUBLIC "${MOAI_ROOT}/src/" "${MOAI_ROOT}/3rdparty/expat-2.0.1/lib")

if ( MOAI_EXPAT )
  target_link_libraries( zlcore expat)
endif ( MOAI_EXPAT )

if ( MOAI_CRYPTO )
  target_link_libraries( zlcore crypto)
endif ( MOAI_CRYPTO )


# compulsory li

if (BUILD_LINUX OR BUILD_WINDOWS OR BUILD_OSX)
  target_link_libraries ( zlcore glew )
endif (BUILD_LINUX OR BUILD_WINDOWS OR BUILD_OSX)

target_link_libraries ( zlcore zlvfs zlib )

if (BUILD_WINDOWS)
  target_link_libraries ( zlcore iphlpapi) #GetAdaptersAddresses
endif (BUILD_WINDOWS)
