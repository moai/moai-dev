cmake_minimum_required ( VERSION 2.8.5 )

#if ( MOAI_CRYPTO )
#  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_LIBCRYPTO=1" )
#  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_LIBCRYPTO=1" )
#message(STATUS "Including crypto ${MOAI_CRYPTO}")
#  find_package ( OpenSSL REQUIRED )
#endif ( MOAI_CRYPTO )
#if ( NOT MOAI_CRYPTO )
#set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_LIBCRYPTO=0" )
#  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_LIBCRYPTO=0" )
	#message(STATUS "Skipping crypto ${MOAI_CRYPTO}")
#endif ( NOT MOAI_CRYPTO )

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
