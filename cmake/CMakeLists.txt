#
# This is the CMake build script for Moai SDK
# Please, don't modify the flags inside this script.
# To create a custom version of Moai SDK use a bash script
# like the ones in bin/ ( bin/build-linux_x64 for example )
#

cmake_minimum_required ( VERSION 2.8.5 )
project ( moai )

# Setup the "MOAI_ROOT" variable
get_filename_component(MOAI_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)

#
# Is this makefile also meant to generate the sample hosts, or just used for the libs
#
option ( BUILD_HOST "Build the bundled sample host for platform" TRUE ) 


#
# Multi-platform flags
#
option ( BUILD_LINUX       FALSE )
option ( BUILD_BLACKBERRY  FALSE )
option ( BUILD_SERVER      FALSE )
option ( BUILD_OSX         FALSE )
option ( BUILD_HTML        FALSE )
option ( BUILD_ANDROID     FALSE )
option ( BUILD_WINDOWS     FALSE )

# We should be adding these soon
#set ( BUILD_NACL        FALSE CACHE STRING "Build Moai SDK for Native Client"     FORCE )

# We won't be adding these to CMake for now:
#set ( BUILD_FLASCC      FALSE CACHE STRING "Build Moai SDK for FlasCC"            FORCE )

#
# Platform Definitions
#
if ( BUILD_OSX )
  find_package ( OpenGL REQUIRED )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -D__APPLE__ -D__MACH__ -DMACOSX -DHAVE_MEMMOVE -D_FORTIFY_SOURCE=0 " )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__APPLE__ -D__MACH__ -DMACOSX" )
endif ( BUILD_OSX )

if ( BUILD_BLACKBERRY )
  # set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -V4.6.3,gcc_ntoarmv7le -w1 -D__QNX__")
  # set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__QNX__ -V4.6.3,gcc_ntoarmv7le_cpp")
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__QNX__ -Wc,-std=gnu99 -DNDEBUG -DHAVE_MEMMOVE ")
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__QNX__" )

  set ( CMAKE_RANLIB $ENV{QNX_HOST}/usr/bin/ntoarmv7-ranlib )
  set ( CMAKE_AR $ENV{QNX_HOST}/usr/bin/ntoarmv7-ar )
  set ( BLACKBERRY_INCLUDES "$ENV{QNX_TARGET}/usr/include;$ENV{QNX_TARGET}/usr/include/freetype2" )
endif ( BUILD_BLACKBERRY )

if ( BUILD_HTML )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_OS_HTML -DGL_ES -D_GNU_SOURCE -s LINKABLE=1 -Wno-warn-absolute-paths" )
  SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_OS_HTML -DGL_ES -D_GNU_SOURCE -s LINKABLE=1 -Wno-warn-absolute-paths"   )
endif (BUILD_HTML )

if ( BUILD_WINDOWS)
  find_package ( OpenGL REQUIRED )
  if (MSVC)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS /MP")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS /MP")
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
     message("Windows Visual Studio Build Detected")
  else ()
     #work around bug with mingw in release mode failing at link time due to different sized duplicate segments! Optimise for size instead
     set(CMAKE_CXX_FLAGS_RELEASE "-Os -DNDEBUG")
     set(CMAKE_C_FLAGS_RELEASE "-Os -DNDEBUG")
     set ( GCC_FORCE_32_BIT TRUE) #lets have 32bit for windows (for luajits sake)
     
     set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32 -std=gnu99 -static -Werror-implicit-function-declaration -DHAVE_MEMMOVE" )
     set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -static" )
  endif(MSVC)
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -DWINVER=0x0501 -D_WIN32_WINNT=0x0501 -D_WIN32_IE=0x0500 " )
  SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWINVER=0x0501 -D_WIN32_WINNT=0x0501 -D_WIN32_IE=0x0500"  )
endif (BUILD_WINDOWS)

if ( BUILD_LINUX )
  find_package ( OpenGL REQUIRED )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DPOSIX -std=gnu99 -DNDEBUG -DHAVE_MEMMOVE" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPOSIX " )
endif ( BUILD_LINUX )

if ( BUILD_ANDROID )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99  -DHAVE_MEMMOVE " )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} " )
endif ( BUILD_ANDROID )

if ( BUILD_IOS )
  set ( CMAKE_OSX_SYSROOT iphoneos )
  set ( CMAKE_OSX_ARCHITECTURES "armv7;i386" )
  set ( CMAKE_XCODE_EFFECTIVE_PLATFORMS "-iphoneos;-iphonesimulator" )
  set ( XCODE_ATTRIBUTE_SDKROOT iphoneos )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99  -DHAVE_MEMMOVE " )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} " )
endif ( BUILD_IOS )

if ( SDL_HOST )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__SDL__" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__SDL__" )
endif ( SDL_HOST )

# Virtual File System Force include
# this should force all of moai to have this include and then we remove it from flags in zl-vfs
# easier than adding it to each project!
if (NOT MSVC)
	set (VFS_INCLUDE "-I${MOAI_ROOT}/src -include ${MOAI_ROOT}/src/zl-vfs/zl_replace.h")
else (NOT MSVC)
	set (VFS_INCLUDE "-I${MOAI_ROOT}/src /FI${MOAI_ROOT}/src/zl-vfs/zl_replace.h")
endif (NOT MSVC)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${VFS_INCLUDE}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${VFS_INCLUDE}")

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
option ( MOAI_SDL            FALSE )
option ( MOAI_LUAJIT         FALSE )

if (PLUGIN_DIR)
  set (MOAI_PLUGINS TRUE)
endif (PLUGIN_DIR)


#
# Moai SDK Extensions
#
option ( MOAI_HTTP_CLIENT    FALSE )



macro(SETUP_MODULE module_name flag_name)
  if (${module_name})
    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D${flag_name}=1" )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D${flag_name}=1" )
  else ()
    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D${flag_name}=0" )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D${flag_name}=0" )
  endif(${module_name})
endmacro(SETUP_MODULE)

SETUP_MODULE(MOAI_BOX2D MOAI_WITH_BOX2D)
SETUP_MODULE(MOAI_BOX2D AKU_WITH_BOX2D)

SETUP_MODULE(MOAI_CHIPMUNK MOAI_WITH_CHIPMUNK)
SETUP_MODULE(MOAI_CHIPMUNK AKU_WITH_CHIPMUNK)

SETUP_MODULE(MOAI_CURL MOAI_WITH_LIBCURL)

SETUP_MODULE(MOAI_UNTZ MOAI_WITH_UNTZ)
SETUP_MODULE(MOAI_UNTZ AKU_WITH_UNTZ)

SETUP_MODULE(MOAI_TINYXML MOAI_WITH_TINYXML)

SETUP_MODULE(MOAI_HTTP_CLIENT MOAI_WITH_HTTP_CLIENT)
SETUP_MODULE(MOAI_HTTP_CLIENT AKU_WITH_HTTP_CLIENT)

SETUP_MODULE(MOAI_EXPAT MOAI_WITH_EXPAT)


SETUP_MODULE(MOAI_CRYPTO MOAI_WITH_LIBCRYPTO)
SETUP_MODULE(MOAI_OPENSSL MOAI_WITH_OPENSSL)

SETUP_MODULE(MOAI_FREETYPE MOAI_WITH_FREETYPE)

SETUP_MODULE(MOAI_LUAEXT AKU_WITH_LUAEXT)
SETUP_MODULE(MOAI_LUAEXT MOAI_WITH_LUAEXT)

SETUP_MODULE(MOAI_SQLITE3 MOAI_WITH_SQLITE)

SETUP_MODULE(MOAI_JSON MOAI_WITH_JANSSON)
SETUP_MODULE(MOAI_SFMT MOAI_WITH_SFMT)
SETUP_MODULE(MOAI_PNG MOAI_WITH_LIBPNG)
SETUP_MODULE(MOAI_JPG MOAI_WITH_LIBJPG)
SETUP_MODULE(MOAI_VORBIS MOAI_WITH_VORBIS)
SETUP_MODULE(MOAI_OGG MOAI_WITH_OGG)

SETUP_MODULE(MOAI_SDL MOAI_WITH_SDL)
SETUP_MODULE(MOAI_MONGOOSE MOAI_WITH_MONGOOSE)
SETUP_MODULE(MOAI_LUAJIT MOAI_WITH_LUAJIT)
SETUP_MODULE(MOAI_PLUGINS AKU_WITH_PLUGINS)

SETUP_MODULE(MOAI_TEST AKU_WITH_TEST)

#Flags for optional modules

FOREACH(DISABLED ${DISABLED_EXT} )
   set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDISABLE_${DISABLED}"  )
   set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDISABLE_${DISABLED}"  )
   set ( DISABLE_${DISABLED} TRUE )
ENDFOREACH()



# Not sure about this one
#set ( MOAI_OPENGLES1      FALSE CACHE STRING "Build Moai SDK with OpenGL ES 1.0 Support" FORCE )



#
# Set as debug
#
set ( DEBUG OFF )

#
# Build testing code
#
set ( TESTING OFF )


#missing cmake modules

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DAKU_WITH_HTTP_SERVER=0"  )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAKU_WITH_HTTP_SERVER=0"  )

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
add_subdirectory ( moai-luaext )
if (MOAI_TEST)
   add_subdirectory ( moai-test)
endif (MOAI_TEST)

if (MOAI_PLUGINS)
  add_subdirectory ( moai-plugins )
endif (MOAI_PLUGINS)

#all the modules...together...having a party.
add_subdirectory ( host-modules )



if (CUSTOM_HOST)
#
# Build an out of source host
#
   add_subdirectory( "${CUSTOM_HOST}" "${CMAKE_CURRENT_BINARY_DIR}/host-custom" )

else (CUSTOM_HOST)

#
#  Built In Hosts
#

if ( BUILD_LINUX OR BUILD_OSX OR BUILD_WINDOWS )
    if ( SDL_HOST )
      add_subdirectory ( host-sdl )
    else ( SDL_HOST ) 
      add_subdirectory ( host-glut )
    endif (SDL_HOST)
endif ( BUILD_LINUX OR BUILD_OSX OR BUILD_WINDOWS)

if ( BUILD_HTML ) 
    add_subdirectory ( host-html )
endif ( BUILD_HTML )

if ( BUILD_ANDROID )
  add_subdirectory ( moai-android )
    add_subdirectory ( host-android )
endif ( BUILD_ANDROID )

if ( BUILD_IOS )
  add_subdirectory ( moai-iphone )
  add_subdirectory ( moai-audiosampler )
     add_subdirectory ( host-ios )
endif ( BUILD_IOS )

if ( BUILD_SERVER )
  add_subdirectory ( moaiext-server )
  add_subdirectory ( http-host )
endif ( BUILD_SERVER )

endif (CUSTOM_HOST)
