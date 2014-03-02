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

  

  set ( UNTZ_SOURCES 

    ${CMAKE_CURRENT_SOURCE_DIR}/AudioMixer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BufferedAudioSource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BufferedAudioSourceThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MemoryAudioSource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UntzSound.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UserAudioSource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/native/win/DShowAudioSource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/native/win/UntzSystem.cpp
    "${MOAI_ROOT}/3rdparty/rtaudio-4.0.8/RtAudio.cpp"
    "${MOAI_ROOT}/3rdparty/rtaudio-4.0.8/include/asio.cpp" 
    "${MOAI_ROOT}/3rdparty/rtaudio-4.0.8/include/asiodrivers.cpp" 
    "${MOAI_ROOT}/3rdparty/rtaudio-4.0.8/include/asiolist.cpp" 
    "${MOAI_ROOT}/3rdparty/rtaudio-4.0.8/include/iasiothiscallresolver.cpp"
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

 target_link_libraries ( untz 
    winmm
    uuid
    dxguid
    strmiids
    dsound
    oleaut32
  )
set_target_properties( untz PROPERTIES FOLDER Third-Party )