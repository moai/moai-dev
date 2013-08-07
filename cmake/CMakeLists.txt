#
# This is the CMake build script for Moai SDK
# Please, don't modify the flags inside this script.
# To create a custom version of Moai SDK use a bash script
# like the ones in bin/ ( bin/build-linux_x64 for example )
#

cmake_minimum_required ( VERSION 2.8.5 )
project ( moai )

#
# Multi-platform flags
#
option ( BUILD_LINUX       FALSE )
option ( BUILD_BLACKBERRY  FALSE )
option ( BUILD_SERVER      FALSE )
option ( BUILD_OSX         FALSE )
option ( BUILD_HTML        FALSE )

# We should be adding these soon
#set ( BUILD_ANDROID     FALSE CACHE STRING "Build Moai SDK for Android"           FORCE )
#set ( BUILD_NACL        FALSE CACHE STRING "Build Moai SDK for Native Client"     FORCE )

# We won't be adding these to CMake for now:
#set ( BUILD_WIN32       FALSE CACHE STRING "Build Moai SDK for Windows"           FORCE )
#set ( BUILD_IOS         FALSE CACHE STRING "Build Moai SDK for iOS"               FORCE )
#set ( BUILD_FLASCC      FALSE CACHE STRING "Build Moai SDK for FlasCC"            FORCE )


#
# Platform Definitions
#
if ( BUILD_OSX )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__APPLE__ -D__MACH__" CACHE INTERNAL "doc string")
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__APPLE__ -D__MACH__" CACHE INTERNAL "doc string")
endif ( BUILD_OSX )

if ( BUILD_BLACKBERRY )
  # set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -V4.6.3,gcc_ntoarmv7le -w1 -D__QNX__")
  # set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__QNX__ -V4.6.3,gcc_ntoarmv7le_cpp")
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__QNX__ -Wc,-std=gnu99 -DNDEBUG -DHAVE_MEMMOVE -DFT2_BUILD_LIBRARY" CACHE INTERNAL "doc string")
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__QNX__" CACHE INTERNAL "doc string")

  set ( CMAKE_RANLIB $ENV{QNX_HOST}/usr/bin/ntoarmv7-ranlib )
  set ( CMAKE_AR $ENV{QNX_HOST}/usr/bin/ntoarmv7-ar )
  set ( BLACKBERRY_INCLUDES "$ENV{QNX_TARGET}/usr/include;$ENV{QNX_TARGET}/usr/include/freetype2" )
endif ( BUILD_BLACKBERRY )

if ( BUILD_HTML )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_OS_HTML -s LINKABLE=1 -DFT2_BUILD_LIBRARY" CACHE INTERNAL "doc string")
  SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_OS_HTML -s LINKABLE=1 -DFT2_BUILD_LIBRARY" CACHE INTERNAL "doc string" )
endif (BUILD_HTML )


if ( BUILD_LINUX )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__SDL__ -DPOSIX -std=gnu99 -DNDEBUG -DHAVE_MEMMOVE -DFT2_BUILD_LIBRARY" CACHE INTERNAL "doc string")
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__SDL__ -DPOSIX " CACHE INTERNAL "doc string")
endif ( BUILD_LINUX )

#
# Options
#
# TODO: Implement 32 bits stuff
#set ( FORCE_LINUX_32    FALSE CACHE STRING "Build for Linux in 32bits"             FORCE )

#
# Optional modules
option ( MOAI_BOX2D          FALSE )
option ( MOAI_CHIPMUNK       FALSE )
option ( MOAI_CURL           FALSE )
option ( MOAI_CRYPTO         FALSE )
option ( MOAI_EXPAT          FALSE )
option ( MOAI_FREETYPE       FALSE )
option ( MOAI_JSON           FALSE )
option ( MOAI_JPG            FALSE )
option ( MOAI_LUAEXT         FALSE )
option ( MOAI_MONGOOSE       FALSE )
option ( MOAI_OGG            FALSE )
option ( MOAI_OPENSSL        FALSE )
option ( MOAI_SQLITE3        FALSE )
option ( MOAI_TINYXML        FALSE )
option ( MOAI_PNG            FALSE )
option ( MOAI_SFMT           FALSE )
option ( MOAI_VORBIS         FALSE )
option ( MOAI_UNTZ           FALSE )

# Not sure about this one
#set ( MOAI_OPENGLES1      FALSE CACHE STRING "Build Moai SDK with OpenGL ES 1.0 Support" FORCE )

#
# Moai SDK Extensions
#
option ( MOAI_HTTP_CLIENT    FALSE )


#
# Set as debug
#
set ( DEBUG OFF )

#
# Build testing code
#
set ( TESTING OFF )

if ( DEBUG )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O4")
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O4")
endif ( DEBUG )


#
# Libraries
#
add_subdirectory ( third-party )
add_subdirectory ( zlcore )

add_subdirectory ( moai-core )
add_subdirectory ( moai-util )
add_subdirectory ( moai-sim )


add_subdirectory ( moai-box2d )
add_subdirectory ( moai-chipmunk )


add_subdirectory ( moai-untz )
add_subdirectory ( moai-http-client )

#
# Hosts
#
if ( BUILD_HTML ) 
  add_subdirectory ( host-html )
endif ( BUILD_HTML )

if ( BUILD_LINUX )
  add_subdirectory ( host-glut )
  add_subdirectory ( moai-test )
  add_subdirectory ( host-test )
endif ( BUILD_LINUX )

if ( BUILD_OSX )
  add_subdirectory ( host-glut )
  add_subdirectory ( moai-test )
  add_subdirectory ( host-test )
endif ( BUILD_OSX )

if ( BUILD_SERVER )
  add_subdirectory ( moaiext-server )
  add_subdirectory ( http-host )
endif ( BUILD_SERVER )
