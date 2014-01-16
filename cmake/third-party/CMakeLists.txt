cmake_minimum_required ( VERSION 2.8.5 )

project ( third-party )





add_subdirectory ( box2d )

message("BOX2d Includes = ${BOX2D_INCLUDES}")
add_subdirectory ( chipmunk )
add_subdirectory ( contrib )
add_subdirectory ( crypto )
add_subdirectory ( curl )
add_subdirectory ( expat )
add_subdirectory ( freetype )
add_subdirectory ( jansson )
add_subdirectory ( jpg )
add_subdirectory ( lua )
add_subdirectory ( luaext )
add_subdirectory ( mongoose )
add_subdirectory ( ogg )
add_subdirectory ( png )
add_subdirectory ( sfmt )
add_subdirectory ( sqlite3 )
add_subdirectory ( ssl )
add_subdirectory ( tinyxml )
add_subdirectory ( tlsf )

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

add_subdirectory ( vorbis )
add_subdirectory ( zlib )

if (BUILD_HTML OR BUILD_ANDROID OR BUILD_IOS)
	set ( GLEW_SOURCES "")
	set ( GLEW_INCLUDES "")
else (BUILD_HTML OR BUILD_ANDROID OR BUILD_IOS)
	add_subdirectory ( glew )
endif (BUILD_HTML OR BUILD_ANDROID OR BUILD_IOS)



set ( THIRD_PARTY_INCLUDES 
  "${MOAI_ROOT}/src/"
  "${MOAI_ROOT}/config-default"
  ${BOX2D_INCLUDES}
  ${CHIPMUNK_INCLUDES}
  ${CONTRIB_INCLUDES}
  ${CRYPTO_INCLUDES}
  ${CURL_INCLUDES}
  ${EXPAT_INCLUDES}
  ${FREETYPE_INCLUDES}
  ${JANSSON_INCLUDES}
  ${JPG_INCLUDES}
  ${LUA_INCLUDES}
  ${LUAEXT_INCLUDES}
  ${MONGOOSE_INCLUDES}
  ${OGG_INCLUDES}
  ${PNG_INCLUDES}
  ${SFMT_INCLUDES}
  ${SQLITE3_INCLUDES}
  ${SSL_INCLUDES}
  ${TINYXML_INCLUDES}
  ${TLSF_INCLUDES}
  ${UNTZ_INCLUDES}
  ${VORBIS_INCLUDES}
  ${ZLIB_INCLUDES}
  ${GLEW_INCLUDES}
  "${MOAI_ROOT}/3rdparty/ooid-0.99/"

  )
set (THIRD_PARTY_INCLUDES ${THIRD_PARTY_INCLUDES} PARENT_SCOPE) #export to rest of project
set (UNTZ_INCLUDES ${UNTZ_INCLUDES} PARENT_SCOPE)

include_directories ( ${THIRD_PARTY_INCLUDES} )

if ( BUILD_BLACKBERRY )
  include_directories ( ${BLACKBERRY_INCLUDES} )
endif ( BUILD_BLACKBERRY)

add_library ( third-party STATIC
  ${BOX2D_SOURCES}
  ${CHIPMUNK_SOURCES}
  ${CONTRIB_SOURCES}
  ${CRYPTO_SOURCES}
  ${CURL_SOURCES}
  ${EXPAT_SOURCES}
  ${FREETYPE_SOURCES}
  ${JANSSON_SOURCES}
  ${JPG_SOURCES}
  ${LUA_SOURCES}
  ${LUAEXT_SOURCES}
  ${MONGOOSE_SOURCES}
  ${OGG_SOURCES}
  ${PNG_SOURCES}
  ${SFMT_SOURCES}
  ${SQLITE3_SOURCES}
  ${TINYXML_SOURCES}
  ${TLSF_SOURCES}
  ${UNTZ_SOURCES}
  ${VORBIS_SOURCES}
  ${ZLIB_SOURCES}
  ${GLEW_SOURCES}
  ${SSL_SOURCES}
)

set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" PARENT_SCOPE)
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" PARENT_SCOPE)

set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DTIXML_USE_STL -DGLEW_STATIC -DSQLITE_ENABLE_COLUMN_METADATA -Dfdatasync=fsync -DDARWIN_NO_CARBON" )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTIXML_USE_STL -DGLEW_STATIC" )
set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty" )

if ( BUILD_ANDROID )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -include ${MOAI_ROOT}/src/zl-vfs/zl_replace.h" )
   set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include ${MOAI_ROOT}/src/zl-vfs/zl_replace.h" ) 
endif ( BUILD_ANDROID )



if (BUILD_WINDOWS AND MOAI_UNTZ)
  target_link_libraries ( third-party 
    winmm
    uuid
    dxguid
    strmiids
    dsound
    oleaut32
  )
endif (BUILD_WINDOWS AND MOAI_UNTZ)

if (MOAI_LUAEXT)
target_link_libraries ( third-party
  luasocket
)
endif (MOAI_LUAEXT)
#glew depends on opengl
if ( GLEW_SOURCES )
   target_link_libraries ( third-party 
   ${OPENGL_LIBRARIES} 
 )
endif (GLEW_SOURCES)
