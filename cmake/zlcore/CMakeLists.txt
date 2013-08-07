cmake_minimum_required ( VERSION 2.8.5 )
project ( zlcore )

# Define source directory
set ( CMAKE_CURRENT_SOURCE_DIR "../../src/" )

 if ( BUILD_LINUX )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DL_ENDIAN -DOPENSSL_NO_DEPRECATED -DOPENSSL_SYSNAME_LINUX -DOPENSSL_NO_RC5 -DOPENSSL_NO_MD2 -DOPENSSL_NO_KRB5 -DOPENSSL_NO_JPAKE -DOPENSSL_NO_STATIC_ENGINE " CACHE INTERNAL "doc string" )
  set( PLATFORM_INCLUDE "../../3rdparty/openssl-1.0.0d/include-linux" )
 endif ( BUILD_LINUX)

 if ( BUILD_WINDOWS )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DL_ENDIAN -DOPENSSL_SYSNAME_WIN32 -DOPENSSL_NO_RC5 -DOPENSSL_NO_MD2 -DOPENSSL_NO_KRB5 -DOPENSSL_NO_JPAKE -DOPENSSL_NO_STATIC_ENGINE " CACHE INTERNAL "doc string" )
  set( PLATFORM_INCLUDE "../../3rdparty/openssl-1.0.0d/include-win32" )
 endif ( BUILD_WINDOWS )


 if ( BUILD_ANDROID )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DL_ENDIAN -DOPENSSL_SYSNAME_ANDROID -DOPENSSL_NO_RC5 -DOPENSSL_NO_MD2 -DOPENSSL_NO_KRB5 -DOPENSSL_NO_JPAKE -DOPENSSL_NO_STATIC_ENGINE " CACHE INTERNAL "doc string" )
  set( PLATFORM_INCLUDE "../../3rdparty/openssl-1.0.0d/include-android" )
 endif ( BUILD_ANDROID )


# Define needed include files
set ( ZLCORE_INCLUDES
  "../../src/config-default/" 
  "../../src/"
  ${PLATFORM_INCLUDE}
  "../../3rdParty/"
  "../../3rdParty/ooid-0.99/"
  "../../3rdParty/expat-2.0.1/lib/"
  "../../3rdParty/tlsf-2.0/"
  
  
CACHE INTERNAL "doc string" )

# Use all cpp files in directory as source code
file ( GLOB SRC_ZLCORE "${CMAKE_CURRENT_SOURCE_DIR}/zl-*/*.cpp" )

include_directories ( ${ZLCORE_INCLUDES} )

add_library ( zlcore STATIC ${SRC_ZLCORE} )

if (BUILD_WINDOWS)
  target_link_libraries ( zlcore iphlpapi) #GetAdaptersAddresses
endif (BUILD_WINDOWS)