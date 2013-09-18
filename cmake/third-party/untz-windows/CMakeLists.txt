cmake_minimum_required ( VERSION 2.8.5 )
project ( untz )

if ( MOAI_UNTZ )
  

  set ( CMAKE_CURRENT_SOURCE_DIR "../../3rdparty/untz/src/" )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_UNTZ=1" CACHE INTERNAL "doc string" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_UNTZ=1" CACHE INTERNAL "doc string" )

  set ( UNTZ_INCLUDES 
    ${SDL_INCLUDES}
    "../../3rdparty/untz/include/"
    "../../3rdparty/untz/src/"
    "../../3rdparty/untz/src/native/win/" 
  CACHE INTERNAL "doc string" )

  

  set ( UNTZ_SOURCES 

    ${CMAKE_CURRENT_SOURCE_DIR}/AudioMixer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BufferedAudioSource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BufferedAudioSourceThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MemoryAudioSource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OggAudioSource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UntzSound.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UserAudioSource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/native/win/DShowAudioSource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/native/win/UntzSystem.cpp
  CACHE INTERNAL "doc string" )


else ( MOAI_UNTZ )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_UNTZ=0" CACHE INTERNAL "doc string" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_UNTZ=0" CACHE INTERNAL "doc string" )

endif ( MOAI_UNTZ )
