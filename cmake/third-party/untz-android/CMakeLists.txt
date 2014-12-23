
cmake_minimum_required ( VERSION 2.8.5 )
project ( untz )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/untz/src/" )

set ( UNTZ_INCLUDES 
  ${SDL_INCLUDES}
  "${MOAI_ROOT}/3rdparty/untz/include/"
  "${MOAI_ROOT}/3rdparty/untz/src/"
  "${MOAI_ROOT}/3rdparty/untz/src/native/android/" 
)
file ( GLOB UNTZ_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/native/android/*.cpp"
)
file ( GLOB UNTZ_HEADERS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/native/android/*.h"
)
list ( REMOVE_ITEM UNTZ_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/native/android/testRiff.cpp")
list ( REMOVE_ITEM UNTZ_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/native/android/testWave.cpp")
if ( NOT MOAI_VORBIS )
  list ( REMOVE_ITEM UNTZ_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/OggAudioSource.cpp")
  list ( REMOVE_ITEM UNTZ_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/OggAudioSource.h")
endif ( NOT MOAI_VORBIS )

add_library ( untz STATIC ${UNTZ_SOURCES} ${UNTZ_HEADERS} )

if (MOAI_VORBIS)
  target_link_libraries ( untz vorbis )
endif (MOAI_VORBIS)

target_include_directories ( untz PUBLIC "$<BUILD_INTERFACE:${UNTZ_INCLUDES}>" )
 install(TARGETS untz EXPORT libmoai ARCHIVE DESTINATION lib)
 install(DIRECTORY ${MOAI_ROOT}/3rdparty/untz/include/ DESTINATION include/untz
          FILES_MATCHING PATTERN "*.h")