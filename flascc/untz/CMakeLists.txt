cmake_minimum_required ( VERSION 2.8.5 )
project ( untz )

set ( UNTZ_INCLUDES "../../3rdparty/untz/include/;../../3rdparty/untz/src/;../../3rdparty/untz/src/native/sdl/" CACHE INTERNAL "doc string" )

include_directories ( 
  # ${MOAICORE_INCLUDES} 
  # ${LUA_INCLUDES} 
  # ${SSL_INCLUDES} 
  # ${EXPAT_INCLUDES} 
  # ${BOX2D_INCLUDES} 
  # ${CHIPMUNK_INCLUDES}
  # ${FREETYPE_INCLUDES}
  # ${TINYXML_INCLUDES}
  # ${PNG_INCLUDES}
  # ${JPG_INCLUDES}
  # ${JANSSON_INCLUDES}
  ${UNTZ_INCLUDES}
  ${VORBIS_INCLUDES}
  ${OGG_INCLUDES}
)

if ( FLASCC_BUILD )
  include_directories ( "../../3rdparty/GLS3D/install/usr/include/")
  include_directories ( "${FLASCC}/usr/include/SDL")
  
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__FLASCC__ -DPOSIX -D__SDL__" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_CURL=false")
endif ( FLASCC_BUILD )

set ( CMAKE_CURRENT_SOURCE_DIR "../../3rdparty/untz/src/" )

add_library ( untz STATIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/AudioMixer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/BufferedAudioSource.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/BufferedAudioSourceThread.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/MemoryAudioSource.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/OggAudioSource.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UntzSound.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UserAudioSource.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/native/sdl/RiffFile.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/native/sdl/UntzSystem.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/native/sdl/WaveFile.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/native/sdl/WaveFileAudioSource.cpp
)