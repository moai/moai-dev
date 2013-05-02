cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-core )

set ( MOAICORE_INCLUDES 
  "../../src/"
  "../../3rdparty/"
  "../../src/config-default/"
  ${LUA_INCLUDES} 
  ${ZLIB_INCLUDES}
  ${GLEW_INCLUDES}
  ${BOX2D_INCLUDES}
  ${SSL_INCLUDES} 
  ${CRYPTO_INCLUDES} 
  ${EXPAT_INCLUDES} 
  ${BOX2D_INCLUDES} 
  ${CHIPMUNK_INCLUDES}
  ${FREETYPE_INCLUDES}
  ${TINYXML_INCLUDES}
  ${PNG_INCLUDES}
  ${JPG_INCLUDES}
  ${JANSSON_INCLUDES}
  ${SFMT_INCLUDES}

 CACHE INTERNAL "doc string" )

include_directories ( 
  ${MOAICORE_INCLUDES} 
)

set ( CMAKE_CURRENT_SOURCE_DIR "../../src/moaicore/" )

# Use all cpp files in directory as source code
file ( GLOB SRC_MOAICORE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )

include_directories ( ${MOAICORE_INCLUDES} )

add_library ( moai-core STATIC ${SRC_MOAICORE} )
