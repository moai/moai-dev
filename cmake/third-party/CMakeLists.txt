cmake_minimum_required ( VERSION 2.8.5 )

project ( third-party )


set ( CMAKE_CURRENT_SOURCE_DIR "${ROOT_DIR}../../../3rdparty" )

add_subdirectory ( box2d )
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
add_subdirectory ( untz )
add_subdirectory ( vorbis )
add_subdirectory ( zlib )

if (BUILD_HTML)
	set ( GLEW_SOURCES "")
	set ( GLEW_INCLUDES "")
else (BUILD_HTML)
	add_subdirectory ( glew )
endif (BUILD_HTML)

set ( THIRD_PARTY_INCLUDES 
  
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
  "../../3rdparty/ooid-0.99/"
  
CACHE INTERNAL "doc string" )

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
)

set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DTIXML_USE_STL" CACHE INTERNAL "doc string")
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTIXML_USE_STL" CACHE INTERNAL "doc string")
