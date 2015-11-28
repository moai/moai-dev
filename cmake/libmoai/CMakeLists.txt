project (libmoai)

add_subdirectory ( config )
set (LUA_LIB_DIR "")
add_subdirectory ( ../third-party third-party )
set (LUA_LIB_DIR ${LUA_LIB_DIR} PARENT_SCOPE)

add_subdirectory ( zlcore )

add_subdirectory ( moai-core )
add_subdirectory ( moai-util )
add_subdirectory ( moai-sim )
add_subdirectory ( moai-crypto )

add_subdirectory ( moai-box2d )
add_subdirectory ( moai-untz )
add_subdirectory ( moai-http-client )
add_subdirectory ( moai-http-server )
add_subdirectory ( moai-luaext )
add_subdirectory ( moai-audiosampler )

add_subdirectory ( moai-image-jpg )
add_subdirectory ( moai-image-pvr )
add_subdirectory ( moai-image-png )
add_subdirectory ( moai-image-webp )

if (MOAI_TEST)
   add_subdirectory ( moai-test)
endif (MOAI_TEST)


add_subdirectory ( moai-plugins )


#platform specific libs
if (BUILD_LINUX OR BUILD_OSX OR BUILD_WINDOWS)
  add_subdirectory ( moai-sdl )
endif (BUILD_LINUX OR BUILD_OSX OR BUILD_WINDOWS)

if ( BUILD_OSX OR BUILD_IOS )
  add_subdirectory ( moai-apple )
endif ( BUILD_OSX OR BUILD_IOS )

if ( BUILD_IOS )
  add_subdirectory ( moai-iphone ) 
endif ( BUILD_IOS )

if ( BUILD_ANDROID )
    add_subdirectory ( moai-android )
endif (BUILD_ANDROID)

