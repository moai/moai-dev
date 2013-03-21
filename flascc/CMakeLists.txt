cmake_minimum_required ( VERSION 2.8.5 )
project ( moai )

#
# Multi-platform flags
#
set ( FLASCC_BUILD    TRUE CACHE STRING "Build Moai SDK for FlasCC" FORCE )

#
# Optional modules
#
set ( MOAI_BOX2D          FALSE CACHE STRING "Build Moai SDK with Box2D Support" FORCE )
set ( MOAI_CHIPMUNK       FALSE CACHE STRING "Build Moai SDK with Chipmunk Support" FORCE )
set ( MOAI_UNTZ           FALSE CACHE STRING "Build Moai SDK with Untz Support" FORCE )

#
# FlasCC-Specific configurations
#
if ( FLASCC_BUILD )
  set ( FLASCC "/Users/nictuku/flascc/sdk")
  set ( JAVA /usr/bin/java )
  set ( FLEX "/Users/nictuku/flex_sdk")
  set ( AS3COMPILER asc2.jar)
  set ( BASE_CFLAGS -Werror -Wno-write-strings -Wno-trigraphs )
  set ( GLS3D "../../../3rdparty/GLS3D" )
endif ( FLASCC_BUILD )


add_subdirectory(third-party)
add_subdirectory(zlcore)
add_subdirectory(uslscore)
# add_subdirectory(untz)

add_subdirectory(moai-core)
# add_subdirectory(moai-untz)

add_subdirectory(aku)

if ( FLASCC_BUILD )
  add_subdirectory(flascc-host)
endif ( FLASCC_BUILD )
