cmake_minimum_required ( VERSION 2.8.5 )
project ( moai )

#
# Multi-platform flags
#
set ( BUILD_LINUX       TRUE  CACHE STRING "Build Moai SDK for Linux"                FORCE )
set ( BUILD_BLACKBERRY  FALSE CACHE STRING "Build Moai SDK for BlackBerry"          FORCE )
set ( BUILD_SERVER      FALSE CACHE STRING "Build Moai SDK with HTTP Server host"   FORCE )
set ( BUILD_OSX         FALSE CACHE STRING "Build Moai SDK for MacOSX"           FORCE )

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

#
# Options
#
# TODO: Implement 32 bits stuff
#set ( FORCE_LINUX_32    FALSE CACHE STRING "Build for Linux in 32bits"             FORCE )

#
# Optional modules
set ( MOAI_BOX2D          TRUE CACHE STRING "Build Moai SDK with Box2D Support"              FORCE )
set ( MOAI_CHIPMUNK       TRUE CACHE STRING "Build Moai SDK with Chipmunk Support"           FORCE )
set ( MOAI_CURL           TRUE  CACHE STRING "Build Moai SDK with CURL Support"               FORCE )
set ( MOAI_CRYPTO         TRUE  CACHE STRING "Build Moai SDK with Crypto Support"             FORCE )
set ( MOAI_EXPAT          TRUE  CACHE STRING "Build Moai SDK with Lua extensions Support"     FORCE )
set ( MOAI_FREETYPE       TRUE  CACHE STRING "Build Moai SDK with FreeType Support"           FORCE )
set ( MOAI_JSON           TRUE  CACHE STRING "Build Moai SDK with JSON Support"               FORCE )
set ( MOAI_JPG            TRUE  CACHE STRING "Build Moai SDK with Jpg Support"                FORCE )
set ( MOAI_LUAEXT         FALSE CACHE STRING "Build Moai SDK with Lua extensions Support"     FORCE )
set ( MOAI_MONGOOSE       TRUE  CACHE STRING "Build Moai SDK with Mongoose Support"           FORCE )
set ( MOAI_OGG            TRUE  CACHE STRING "Build Moai SDK with Png Support"                FORCE )
set ( MOAI_OPENSSL        FALSE CACHE STRING "Build Moai SDK with OpenSSL Support"            FORCE )
set ( MOAI_SQLITE3        TRUE  CACHE STRING "Build Moai SDK with SQLite3 Support"            FORCE )
set ( MOAI_TINYXML        TRUE  CACHE STRING "Build Moai SDK with TinyXML Support"            FORCE )
set ( MOAI_PNG            TRUE  CACHE STRING "Build Moai SDK with Png Support"                FORCE )
set ( MOAI_SFMT           TRUE  CACHE STRING "Build Moai SDK with SFMT Support"               FORCE )
set ( MOAI_VORBIS         TRUE  CACHE STRING "Build Moai SDK with Vorbis Support"             FORCE )

# Not sure about this one
#set ( MOAI_OPENGLES1      FALSE CACHE STRING "Build Moai SDK with OpenGL ES 1.0 Support" FORCE )

#
# Moai SDK Extensions
#
set ( MOAI_HTTP_CLIENT    FALSE CACHE STRING "Build Moai SDK with HTTP Client" FORCE )
set ( MOAI_UNTZ           FALSE CACHE STRING "Build Moai SDK with Untz Support" FORCE )


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
if ( BUILD_LINUX )
  add_subdirectory ( host-glut )
endif ( BUILD_LINUX )

if ( BUILD_OSX )
  add_subdirectory ( host-glut )
endif ( BUILD_OSX )

if ( BUILD_SERVER )
  add_subdirectory ( moaiext-server )
  add_subdirectory ( http-host )
endif ( BUILD_SERVER )
