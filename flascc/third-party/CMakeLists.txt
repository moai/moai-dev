cmake_minimum_required ( VERSION 2.8.5 )

project ( third-party )

set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -DNDEBUG -DHAVE_MEMMOVE -DFT2_BUILD_LIBRARY" )

set ( CMAKE_CURRENT_SOURCE_DIR "${ROOT_DIR}../../../3rdparty" )

set ( THIRD_PARTY_INCLUDES 
  
  ${BOX2D_INCLUDES}
  ${CHIPMUNK_INCLUDES}
  ${CRYPTO_INCLUDES}
  ${EXPAT_INCLUDES}
  ${FREETYPE_INCLUDES}
  ${JANSSON_INCLUDES}
  ${LUAEXT_INCLUDES}
  ${OGG_INCLUDES}
  ${SQLITE3_INCLUDES}
  ${TINYXML_INCLUDES}
  ${VORBIS_INCLUDES}
  
CACHE INTERNAL "doc string" )

if ( FLASCC_BUILD )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__FLASCC__" )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNO_WINDOWS_BRAINDEATH" ) # Crypto
endif ( FLASCC_BUILD )

# add_subdirectory ( ssl )

#add_subdirectory ( box2d )
#add_subdirectory ( chipmunk )
add_subdirectory ( contrib )
#add_subdirectory ( crypto )
#add_subdirectory ( expat )
#add_subdirectory ( freetype )
#add_subdirectory ( jansson )
#add_subdirectory ( jpg )
add_subdirectory ( lua )
#add_subdirectory ( luaext )
#add_subdirectory ( ogg )
#add_subdirectory ( png )
#add_subdirectory ( sfmt )
#add_subdirectory ( sqlite3 )
#add_subdirectory ( tinyxml )
add_subdirectory ( tlsf )
#add_subdirectory ( vorbis )
add_subdirectory ( zlib )

include_directories ( 
  ${THIRD_PARTY_INCLUDES}
)

add_library ( third-party STATIC 
  #${BOX2D_SOURCES}
  #${CHIPMUNK_SOURCES}
  ${CONTRIB_SOURCES}
  #${CRYPTO_SOURCES}
  #${EXPAT_SOURCES}
  #${FREETYPE_SOURCES}
  #${JANSSON_SOURCES}
  #${JPG_SOURCES}
  ${LUA_SOURCES} 
  #${LUAEXT_SOURCES} 
  #${OGG_SOURCES} 
  #${PNG_SOURCES} 
  #${SFMT_SOURCES}
  #${SQLITE3_SOURCES} 
  #${TINYXML_SOURCES}
  ${TLSF_SOURCES}
  #${VORBIS_SOURCES}
  ${ZLIB_SOURCES}
)