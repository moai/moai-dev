cmake_minimum_required ( VERSION 2.8.5 )
project ( untz )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/untz/src/" )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__SDL__")
set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__SDL__")

set ( UNTZ_INCLUDES 
  ${SDL_INCLUDES}
  "${MOAI_ROOT}/3rdparty/untz/include/"
  "${MOAI_ROOT}/3rdparty/untz/src/"
  "${MOAI_ROOT}/3rdparty/untz/src/native/sdl/" 
)
file ( GLOB UNTZ_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/native/sdl/*.cpp"
)
file ( GLOB UNTZ_HEADERS
  "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/native/sdl/*.h"
)
if ( NOT MOAI_VORBIS )
  list ( REMOVE_ITEM UNTZ_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/OggAudioSource.cpp")
  list ( REMOVE_ITEM UNTZ_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/OggAudioSource.h")
endif ( NOT MOAI_VORBIS )

add_library ( untz STATIC ${UNTZ_SOURCES} ${UNTZ_HEADERS} )

target_link_libraries( untz SDL2-static )

if (MOAI_VORBIS)
  target_link_libraries ( untz vorbis )
endif (MOAI_VORBIS)

target_include_directories ( untz PUBLIC "$<BUILD_INTERFACE:${UNTZ_INCLUDES}>" )

set_target_properties( untz PROPERTIES FOLDER Third-Party )
 install(TARGETS untz EXPORT libmoai ARCHIVE DESTINATION lib)

install(DIRECTORY ${MOAI_ROOT}/3rdparty/untz/include/ DESTINATION include/untz
          FILES_MATCHING PATTERN "*.h")