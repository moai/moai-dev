cmake_minimum_required ( VERSION 2.8.11 )
project ( host-modules )


# Use all cpp files in directory as source code
file ( GLOB HOST_MODULES_SRC
  "${HOST_ROOT}/host-modules/aku_modules.*"
  "${HOST_ROOT}/host-modules/aku_modules_util.*"
  "${HOST_ROOT}/host-modules/aku_modules_config.h"
)


add_library ( host-modules STATIC ${HOST_MODULES_SRC} )
set_target_properties( host-modules PROPERTIES FOLDER Modules )
target_include_directories(host-modules PUBLIC ${HOST_ROOT})

  target_link_libraries ( host-modules 
  						  moai-box2d  
  						  moai-untz  
  						  moai-http-client 
  						  moai-luaext 
  						  moai-core
                moai-crypto
                 )

