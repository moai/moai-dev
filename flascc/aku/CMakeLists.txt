cmake_minimum_required ( VERSION 2.8.5 )
project ( aku )

# set ( AKU_INCLUDES "../../src/;../../3rdparty/;../../src/config-default/" )

include_directories ( 
  ${MOAICORE_INCLUDES} 
  ${LUA_INCLUDES} 
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

if ( FLASCC_BUILD )
  include_directories ( "../../3rdparty/GLS3D/install/usr/include/")
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__FLASCC__" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__FLASCC__" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_CURL=false")
endif ( FLASCC_BUILD )

set ( CMAKE_CURRENT_SOURCE_DIR "../../src/aku/" )

add_library ( aku STATIC 
  # ${CMAKE_CURRENT_SOURCE_DIR}/AKU-debugger.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/AKU-luaext.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/AKU-particles.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/AKU-test.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/AKU-untz.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/AKU.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp
)