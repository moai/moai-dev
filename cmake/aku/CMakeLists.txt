cmake_minimum_required ( VERSION 2.8.5 )
project ( aku )

include_directories ( 
  ${MOAICORE_INCLUDES} 
  ${LUA_INCLUDES} 
  ${ZLIB_INCLUDES}
  ${GLEW_INCLUDES}
  ${SSL_INCLUDES} 
  ${EXPAT_INCLUDES} 
  ${BOX2D_INCLUDES} 
  ${CHIPMUNK_INCLUDES}
  ${FREETYPE_INCLUDES}
  ${TINYXML_INCLUDES}
  ${PNG_INCLUDES}
  ${JPG_INCLUDES}
  ${JANSSON_INCLUDES}
  ${UNTZ_INCLUDES}
)

set ( CMAKE_CURRENT_SOURCE_DIR "../../src/aku/" )

# Use all cpp files in directory as source code
file ( GLOB SRC_AKU "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )

add_library ( aku STATIC ${SRC_AKU} )
