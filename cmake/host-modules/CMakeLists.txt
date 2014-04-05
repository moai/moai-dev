cmake_minimum_required ( VERSION 2.8.11 )
project ( host-modules)

set ( HOST_MODULES_INCLUDES 
  "${MOAI_ROOT}/src"
  "${MOAI_ROOT}/src/config-default"
)

# Use all cpp files in directory as source code
file ( GLOB HOST_MODULES_SRC
  "${MOAI_ROOT}/src/host-modules/*.cpp"
  "${MOAI_ROOT}/src/host-modules/*.h"
)

include_directories ( ${HOST_MODULES_INCLUDES} )

add_library ( host-modules STATIC ${HOST_MODULES_SRC} )
set_target_properties( host-modules PROPERTIES FOLDER Modules )
  
#BOX2D
if ( MOAI_BOX2D )
  target_link_libraries ( host-modules moai-box2d )
endif ( MOAI_BOX2D )

#UNTZ
if ( MOAI_UNTZ )
  target_link_libraries ( host-modules moai-untz )
endif ( MOAI_UNTZ )

#Chipmunk
if ( MOAI_CHIPMUNK )
  target_link_libraries ( host-modules moai-chipmunk )
endif ( MOAI_CHIPMUNK )

#Http Client
if ( MOAI_HTTP_CLIENT )
  target_link_libraries ( host-modules moai-http-client )
endif ( MOAI_HTTP_CLIENT )

#LuaExt
if ( MOAI_LUAEXT )
  target_link_libraries ( host-modules moai-luaext )
endif ( MOAI_LUAEXT )

#plugins
if ( MOAI_PLUGINS )
	target_link_libraries ( host-modules moai-plugins )
endif ( MOAI_PLUGINS )

#tests
if ( MOAI_TEST )
	target_link_libraries ( host-modules moai-test )
endif (MOAI_TEST)