# cmake_minimum_required ( VERSION 2.8.5 )
# project ( untz )
# 
# if ( MOAI_UNTZ )
# 
#   
#   
#   
#   include_directories ( 
#     ${SDL_INCLUDES}
#     ${UNTZ_INCLUDES}
#     ${VORBIS_INCLUDES}
#     ${OGG_INCLUDES}
#   )
#   
#   
#   add_library ( untz STATIC 
#   )
# 
# endif ( MOAI_UNTZ )

cmake_minimum_required ( VERSION 2.8.5 )
project ( untz )

if ( MOAI_UNTZ )
if (NOT BUILD_HTML)
  if ( MOAI_SDL OR BUILD_BLACKBERRY )
    # TODO: Use 3rdparty's SDL
  
  
  else ( MOAI_SDL OR BUILD_BLACKBERRY )
    find_package(PkgConfig)
    pkg_check_modules(SDL REQUIRED sdl)
    set ( SDL_INCLUDES "${SDL_INCLUDE_DIRS}" CACHE INTERNAL "" )
    set ( SDL_LIBRARY "${SDL_LIBRARIES}" CACHE INTERNAL "" )
  endif ( MOAI_SDL OR BUILD_BLACKBERRY )

  set ( CMAKE_CURRENT_SOURCE_DIR "../../../3rdparty/untz/src/" )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_UNTZ=1" CACHE INTERNAL "doc string" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_UNTZ=1" CACHE INTERNAL "doc string" )

  set ( UNTZ_INCLUDES 
    ${SDL_INCLUDES}
    "../../3rdparty/untz/include/"
    "../../3rdparty/untz/src/"
    "../../3rdparty/untz/src/native/sdl/" 
  CACHE INTERNAL "doc string" )

  if ( BUILD_BLACKBERRY )
    set ( UNTZ_INCLUDES "${UNTZ_INCLUDES};../../3rdparty/SDL/include/" CACHE INTERNAL "doc string" )
  endif ( BUILD_BLACKBERRY )

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

  CACHE INTERNAL "doc string" )
else (NOT BUILD_HTML)
    set ( UNTZ_INCLUDES "../../3rdparty/untz/include/;../../3rdparty/untz/src/;../../3rdparty/untz/src/native/sdl/" CACHE INTERNAL "doc string" )
	include_directories (
		${UNTZ_INCLUDES}	
	)
	set ( CMAKE_CURRENT_SOURCE_DIR "../../../3rdparty/untz/webuntz/" )

	add_library ( untz STATIC 
  		${CMAKE_CURRENT_SOURCE_DIR}/UntzSound.cpp
  		${CMAKE_CURRENT_SOURCE_DIR}/UntzSystem.cpp )      
	
endif (NOT BUILD_HTML)

else ( MOAI_UNTZ )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_UNTZ=0" CACHE INTERNAL "doc string" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_UNTZ=0" CACHE INTERNAL "doc string" )

endif ( MOAI_UNTZ )
