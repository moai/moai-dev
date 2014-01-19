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


if ( MOAI_EXPAT )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_EXPAT=1 " PARENT_SCOPE )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_EXPAT=1 " PARENT_SCOPE )
  target_link_libraries( zlcore expat)
else ( MOAI_EXPAT )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_EXPAT=0" PARENT_SCOPE )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_EXPAT=0" PARENT_SCOPE )
endif ( MOAI_EXPAT )

if ( MOAI_CRYPTO )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_LIBCRYPTO=1" PARENT_SCOPE )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_LIBCRYPTO=1" PARENT_SCOPE )
  target_link_libraries( zlcore crypto)
else ( MOAI_CRYPTO )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_LIBCRYPTO=1" PARENT_SCOPE )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_LIBCRYPTO=1" PARENT_SCOPE )
endif ( MOAI_CRYPTO )


# compulsory li

if (BUILD_LINUX OR BUILD_WINDOWS OR BUILD_OSX)
  target_link_libraries ( zlcore glew )
endif (BUILD_LINUX OR BUILD_WINDOWS OR BUILD_OSX)

target_link_libraries ( zlcore zlvfs zlib )

if (BUILD_WINDOWS)
  target_link_libraries ( zlcore iphlpapi) #GetAdaptersAddresses
endif (BUILD_WINDOWS)
