cmake_minimum_required ( VERSION 2.8.5 )
project ( untz )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/untz/src/" )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__WINDOWS_DS__" )

set ( UNTZ_INCLUDES 
  "${MOAI_ROOT}/3rdparty/untz/include/"
  "${MOAI_ROOT}/3rdparty/untz/src/"
  "${MOAI_ROOT}/3rdparty/untz/src/native/win/"
  "${MOAI_ROOT}/3rdparty/rtaudio-4.0.8/" 
  "${MOAI_ROOT}/3rdparty/rtaudio-4.0.8/include/"
)


file ( GLOB UNTZ_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/native/win/*.cpp"
  "${MOAI_ROOT}/3rdparty/rtaudio-4.0.8/*.cpp"
  "${MOAI_ROOT}/3rdparty/rtaudio-4.0.8/include/*.cpp"
)
file ( GLOB UNTZ_HEADERS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/native/win/*.h"
  "${MOAI_ROOT}/3rdparty/rtaudio-4.0.8/*.h"
  "${MOAI_ROOT}/3rdparty/rtaudio-4.0.8/include/*.h"
)
if ( NOT MOAI_VORBIS )
  list ( REMOVE_ITEM UNTZ_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/OggAudioSource.cpp")
  list ( REMOVE_ITEM UNTZ_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/OggAudioSource.h")
endif ( NOT MOAI_VORBIS )

add_library (untz STATIC ${UNTZ_SOURCES})

if (MOAI_VORBIS)
  target_link_libraries (untz vorbis)
endif (MOAI_VORBIS)

target_include_directories ( untz PUBLIC ${UNTZ_INCLUDES})

target_link_libraries ( untz 
  winmm
  uuid
  dxguid
  strmiids
  dsound
  oleaut32
)
set_target_properties( untz PROPERTIES FOLDER Third-Party )
