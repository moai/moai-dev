
cmake_minimum_required ( VERSION 2.8.5 )
project ( untz )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/untz/src/" )

set ( UNTZ_INCLUDES 
  "${MOAI_ROOT}/3rdparty/untz/include/"
  "${MOAI_ROOT}/3rdparty/untz/src/"
  "${MOAI_ROOT}/3rdparty/untz/src/native/ios/" 
)
file ( GLOB UNTZ_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/native/ios/*.cpp"
)
file ( GLOB UNTZ_HEADERS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/native/ios/*.h"
)
if ( NOT MOAI_VORBIS )
  list ( REMOVE_ITEM UNTZ_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/OggAudioSource.cpp")
  list ( REMOVE_ITEM UNTZ_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/OggAudioSource.h")
endif ( NOT MOAI_VORBIS )

add_library (untz STATIC ${UNTZ_SOURCES} ${UNTZ_HEADERS} )

if (MOAI_VORBIS)
  target_link_libraries (untz vorbis)
endif (MOAI_VORBIS)

target_include_directories ( untz PUBLIC ${UNTZ_INCLUDES} )
set_target_properties( untz PROPERTIES FOLDER Third-Party )
