cmake_minimum_required ( VERSION 2.8.11 )
project ( host-modules)


# Use all cpp files in directory as source code
file ( GLOB HOST_MODULES_SRC
  "${HOST_ROOT}/host-modules/aku_modules.*"
  "${HOST_ROOT}/host-modules/aku_modules_config.h"
  "${HOST_ROOT}/host-modules/aku_modules_util.*"
)

#include_directories ( ${HOST_MODULES_INCLUDES} )

add_library ( host-modules STATIC ${HOST_MODULES_SRC} )
set_target_properties( host-modules PROPERTIES FOLDER Modules )
 target_include_directories(host-modules PUBLIC ${HOST_ROOT})
 
# link against all of these if we have a lib path since there are no speed gains to be made
# and so that the project can rely on the defines in aku_modules_config
# and let the linker sort out the redundant entries

if (MOAI_CRYPTO OR LIB_PATH)
   target_link_libraries ( host-modules moai-crypto )
endif (MOAI_CRYPTO OR LIB_PATH)

#BOX2D
if ( MOAI_BOX2D OR LIB_PATH)
  target_link_libraries ( host-modules moai-box2d )
endif ( MOAI_BOX2D OR LIB_PATH)

#UNTZ
if ( MOAI_UNTZ OR LIB_PATH)
  target_link_libraries ( host-modules moai-untz )
endif ( MOAI_UNTZ OR LIB_PATH)

#SDL
if ( MOAI_SDL OR LIB_PATH)
  target_link_libraries ( host-modules moai-sdl )
endif ( MOAI_SDL OR LIB_PATH)

#Http Client
if ( MOAI_HTTP_CLIENT OR LIB_PATH)
  target_link_libraries ( host-modules moai-http-client )
endif ( MOAI_HTTP_CLIENT OR LIB_PATH)

#Http Server
if ( MOAI_HTTP_CLIENT OR LIB_PATH)
  target_link_libraries ( host-modules moai-http-server )
endif ( MOAI_HTTP_CLIENT OR LIB_PATH)

#LuaExt
if ( MOAI_LUAEXT OR LIB_PATH)
  target_link_libraries ( host-modules moai-luaext )
endif ( MOAI_LUAEXT OR LIB_PATH)

#audiosampler
  target_link_libraries ( host-modules moai-audiosampler)

if ( MOAI_IMAGE_JPG OR LIB_PATH)
  target_link_libraries ( host-modules moai-image-jpg )
endif ()
  
if ( MOAI_IMAGE_PNG OR LIB_PATH)
  target_link_libraries ( host-modules moai-image-png )
endif ()

if ( MOAI_IMAGE_PVR OR LIB_PATH)
  target_link_libraries ( host-modules moai-image-pvr )
endif ()

if ( MOAI_IMAGE_WEBP OR LIB_PATH)
  target_link_libraries ( host-modules moai-image-webp )
endif ()


#plugins
if ( MOAI_PLUGINS )
	target_link_libraries ( host-modules moai-plugins )
endif ( MOAI_PLUGINS )

#tests
if ( MOAI_TEST )
	target_link_libraries ( host-modules moai-test )
endif (MOAI_TEST)

target_link_libraries ( host-modules moai-core )



