cmake_minimum_required ( VERSION 2.8.5 )

project ( third-party )

add_subdirectory ( box2d )
add_subdirectory ( chipmunk )
add_subdirectory ( contrib )
add_subdirectory ( crypto )
add_subdirectory ( curl )
add_subdirectory ( cares )
add_subdirectory ( expat )
add_subdirectory ( freetype )
add_subdirectory ( jansson )
add_subdirectory ( jpg )

set (LUA_HEADERS "")
#Lua or LuaJit?
if (MOAI_LUAJIT)
#	if ( BUILD_IOS )#
#		set ( LUAJIT_DISABLE_JIT CACHE bool "Disable JIT." OFF )
#	endif (BUILD_IOS )
	set ( GCC_FORCE_32_BIT TRUE)
	add_subdirectory( luajit )
else()
	add_subdirectory ( lua )
endif (MOAI_LUAJIT)
set (LUA_HEADERS ${LUA_HEADERS} PARENT_SCOPE)




add_subdirectory ( luaext )
add_subdirectory ( mongoose )
add_subdirectory ( ogg )
add_subdirectory ( png )
add_subdirectory ( sfmt )
add_subdirectory ( sqlite3 )
add_subdirectory ( ssl )
add_subdirectory ( tinyxml )
add_subdirectory ( tlsf )
add_subdirectory ( vorbis )
add_subdirectory ( zlib )




#SDL is for pc only
if (BUILD_LINUX OR BUILD_OSX OR BUILD_WINDOWS)
  set ( GCC_FORCE_32_BIT   TRUE)
  set(DIRECTX OFF CACHE BOOL "Use some expat option")
  add_subdirectory("${MOAI_ROOT}/3rdparty/sdl2-2.0.0" "${CMAKE_CURRENT_BINARY_DIRECTORY}/sdl2")
  add_subdirectory ( freeglut )
  add_subdirectory ( glew )
endif (BUILD_LINUX OR BUILD_OSX OR BUILD_WINDOWS)

#Build platform specific untz
if (BUILD_ANDROID)
  add_subdirectory (untz-android)
elseif (BUILD_IOS OR BUILD_OSX)
  add_subdirectory (untz-ios)
elseif (BUILD_WINDOWS)
  add_subdirectory (untz-windows)
elseif (BUILD_HTML)
  add_subdirectory (untz-html)
else ()
 add_subdirectory ( untz )
endif ()




#if ( BUILD_ANDROID )
#  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -include ${MOAI_ROOT}/src/zl-vfs/zl_replace.h" )
#   set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include ${MOAI_ROOT}/src/zl-vfs/zl_replace.h" ) 
#endif ( BUILD_ANDROID )

