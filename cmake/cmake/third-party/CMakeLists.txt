cmake_minimum_required ( VERSION 2.8.5 )

project ( third-party )

if (MSVC)
  #this is someone elses code, lets not get spammed by their warnings
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W0")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0")
endif (MSVC)

add_subdirectory(assimp)

if (BUILD_ANDROID)
  add_subdirectory(android)
endif(BUILD_ANDROID)


add_subdirectory ( box2d )
add_subdirectory ( contrib )
add_subdirectory ( curl )

if (BUILD_IOS OR BUILD_ANDROID)
	add_subdirectory ( cares )
endif (BUILD_IOS OR BUILD_ANDROID)
add_subdirectory ( expat )
add_subdirectory ( freetype )
add_subdirectory ( harfbuzz )
add_subdirectory ( jansson )
add_subdirectory ( jpg )
add_subdirectory ( kissfft )

set (LUA_HEADERS "")
set (LUA_LIB "")
set (LUA_LIB_DIR "")
#Lua or LuaJit?
if (MOAI_LUAJIT)
	add_subdirectory( luajit )
else()
	add_subdirectory ( lua )
endif (MOAI_LUAJIT)
set (LUA_HEADERS ${LUA_HEADERS} PARENT_SCOPE)
set (LUA_LIB ${LUA_LIB} PARENT_SCOPE)
set (LUA_LIB_DIR ${LUA_LIB_DIR} PARENT_SCOPE)

add_subdirectory ( luaext )
add_subdirectory ( mbedtls )
add_subdirectory ( mongoose )
add_subdirectory ( ogg )
add_subdirectory ( png )
add_subdirectory ( pvr )
add_subdirectory ( sfmt )
add_subdirectory ( sqlite3 )


add_subdirectory ( tess )
add_subdirectory ( tinyxml )
add_subdirectory ( tlsf )
add_subdirectory ( vorbis )
add_subdirectory ( webp )
add_subdirectory ( zlib )

#SDL is for pc only
if (BUILD_LINUX OR BUILD_OSX OR BUILD_WINDOWS)
  add_subdirectory( sdl2 )
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

