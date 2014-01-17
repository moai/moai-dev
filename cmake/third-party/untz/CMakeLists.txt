cmake_minimum_required ( VERSION 2.8.5 )
project ( untz )

if ( MOAI_UNTZ )
  if ( MOAI_SDL OR BUILD_BLACKBERRY )
    # TODO: Use 3rdparty's SDL
  
  
  else ( MOAI_SDL OR BUILD_BLACKBERRY )
    find_package(PkgConfig)
    pkg_check_modules(SDL REQUIRED sdl)
    set ( SDL_INCLUDES "${SDL_INCLUDE_DIRS}" CACHE INTERNAL "" )
    set ( SDL_LIBRARY "${SDL_LIBRARIES}" CACHE INTERNAL "" )
  endif ( MOAI_SDL OR BUILD_BLACKBERRY )

  set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/untz/src/" )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_UNTZ=1" PARENT_SCOPE )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_UNTZ=1" PARENT_SCOPE )

  set ( UNTZ_INCLUDES 
    ${SDL_INCLUDES}
    "${MOAI_ROOT}/3rdparty/untz/include/"
    "${MOAI_ROOT}/3rdparty/untz/src/"
    "${MOAI_ROOT}/3rdparty/untz/src/native/sdl/" 
   )

  if ( BUILD_BLACKBERRY )
    set ( UNTZ_INCLUDES "${UNTZ_INCLUDES};${MOAI_ROOT}/3rdparty/SDL/include/" PARENT_SCOPE )
  endif ( BUILD_BLACKBERRY )

  set ( UNTZ_INCLUDES ${UNTZ_INCLUDES} PARENT_SCOPE)

  set ( UNTZ_SOURCES 

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

  PARENT_SCOPE )


else ( MOAI_UNTZ )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_UNTZ=0" PARENT_SCOPE )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_UNTZ=0" PARENT_SCOPE )

endif ( MOAI_UNTZ )
