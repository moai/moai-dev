cmake_minimum_required ( VERSION 2.8.5 )
project ( moai )

#
# Multi-platform flags
#
set ( FLASCC_BUILD    true )


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
add_subdirectory(untz)

add_subdirectory(moai-core)
add_subdirectory(moai-untz)

add_subdirectory(aku)

if ( FLASCC_BUILD )
  add_subdirectory(flascc-host)
endif ( FLASCC_BUILD )
