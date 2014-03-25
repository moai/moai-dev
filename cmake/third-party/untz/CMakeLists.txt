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

  set ( UNTZ_INCLUDES ${UNTZ_INCLUDES} )

  set ( UNTZ_SOURCES 

    ${CMAKE_CURRENT_SOURCE_DIR}/AudioMixer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BufferedAudioSource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BufferedAudioSourceThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MemoryAudioSource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UntzSound.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UserAudioSource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/native/sdl/RiffFile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/native/sdl/UntzSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/native/sdl/WaveFile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/native/sdl/WaveFileAudioSource.cpp

  )

  if (MOAI_VORBIS)   
     set ( UNTZ_SOURCES 
       ${UNTZ_SOURCES}
       ${CMAKE_CURRENT_SOURCE_DIR}/OggAudioSource.cpp
      )
  endif (MOAI_VORBIS)

  add_library (untz STATIC ${UNTZ_SOURCES})

  target_link_libraries( untz SDL2-static )

  if (MOAI_VORBIS)
    target_link_libraries (untz vorbis)
  endif (MOAI_VORBIS)

target_include_directories ( untz PUBLIC ${UNTZ_INCLUDES})
set_target_properties( untz PROPERTIES FOLDER Third-Party )
