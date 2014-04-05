cmake_minimum_required ( VERSION 2.8.5 )
project ( untz )


set ( UNTZ_INCLUDES "${MOAI_ROOT}/3rdparty/untz/include/;${MOAI_ROOT}/3rdparty/untz/src/;${MOAI_ROOT}/3rdparty/untz/src/native/sdl/" )
include_directories (
  ${UNTZ_INCLUDES}	
)
set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/untz/webuntz/" )

set( UNTZ_SOURCES 
  ${CMAKE_CURRENT_SOURCE_DIR}/UntzSound.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UntzSystem.cpp 
)

add_library ( untz STATIC ${UNTZ_SOURCES} )
target_include_directories ( untz PUBLIC ${UNTZ_INCLUDES} )	
