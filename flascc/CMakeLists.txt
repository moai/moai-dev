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
set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O4 " )
set ( CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} -O4 " )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O4 " )

#
# FlasCC-Specific configurations
#
if ( FLASCC_BUILD )
  set ( FLASCC "C:/flascc/sdk")
  set ( JAVA java )
  set ( FLEX "/cygdrive/c/flex_sdk" )

  set ( AS3COMPILER "${FLASCC}/usr/lib/asc2.jar" )

  set ( BASE_CFLAGS -Werror -Wno-write-strings -Wno-trigraphs )
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
