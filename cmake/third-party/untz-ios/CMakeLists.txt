
cmake_minimum_required ( VERSION 2.8.5 )
project ( untz )

  set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/untz/src/" )


  set ( UNTZ_INCLUDES 
    "${MOAI_ROOT}/3rdparty/untz/include/"
    "${MOAI_ROOT}/3rdparty/untz/src/"
    "${MOAI_ROOT}/3rdparty/untz/src/native/ios/" 
  )

  

  set ( UNTZ_SOURCES 

    ${CMAKE_CURRENT_SOURCE_DIR}/AudioMixer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BufferedAudioSource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BufferedAudioSourceThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MemoryAudioSource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UntzSound.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UserAudioSource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/native/ios/ExtAudioFileAudioSource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/native/ios/UntzSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/native/ios/vector_util.cpp
  )


if (MOAI_VORBIS)   
     set ( UNTZ_SOURCES 
       ${UNTZ_SOURCES}
       ${CMAKE_CURRENT_SOURCE_DIR}/OggAudioSource.cpp
      )
   endif (MOAI_VORBIS)

add_library (untz STATIC ${UNTZ_SOURCES})

if (MOAI_VORBIS)
  target_link_libraries (untz vorbis)
endif (MOAI_VORBIS)

target_include_directories ( untz PUBLIC ${UNTZ_INCLUDES})