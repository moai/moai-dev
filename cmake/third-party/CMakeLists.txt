cmake_minimum_required ( VERSION 2.8.5 )

project ( third-party )

if (MSVC)
  #this is someone elses code, lets not get spammed by their warnings
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W0")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0")
endif (MSVC)

add_subdirectory ( box2d )
add_subdirectory ( chipmunk )
add_subdirectory ( contrib )
add_subdirectory ( crypto )
add_subdirectory ( curl )

if (BUILD_IOS OR BUILD_ANDROID)
	add_subdirectory ( cares )
endif (BUILD_IOS OR BUILD_ANDROID)
add_subdirectory ( expat )
add_subdirectory ( freetype )
add_subdirectory ( jansson )
add_subdirectory ( jpg )

set (LUA_HEADERS "")
set (LUA_LIB "")
#Lua or LuaJit?
if (MOAI_LUAJIT)
#	if ( BUILD_IOS )#
#		set ( LUAJIT_DISABLE_JIT CACHE bool "Disable JIT." OFF )
#	endif (BUILD_IOS )
	add_subdirectory( luajit )
else()
	add_subdirectory ( lua )
endif (MOAI_LUAJIT)
set (LUA_HEADERS ${LUA_HEADERS} PARENT_SCOPE)
set (LUA_LIB ${LUA_LIB} PARENT_SCOPE)

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
  set(DIRECTX OFF CACHE BOOL "SDL DirectX")
  set (SDL_SHARED OFF)
  
  add_subdirectory(sdl2)
  if (BUILD_LINUX OR BUILD_WINDOWS)
  add_subdirectory ( freeglut )
  endif (BUILD_LINUX OR BUILD_WINDOWS)
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

