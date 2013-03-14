cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-untz )

set ( CMAKE_CURRENT_SOURCE_DIR "../../src/moaiext-untz/" )
set ( MOAIUNTZ_INCLUDES "../../src/;../../src/moaiext-untz/" CACHE INTERNAL "doc string" )

if ( FLASCC_BUILD )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_CURL=false -DUSE_OPENSSL=false")
  include_directories ( "../../3rdparty/GLS3D/install/usr/include/")
  include_directories ( "${FLASCC}/usr/include/SDL")
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__FLASCC__ -DPOSIX -D__SDL__" )
    
endif ( FLASCC_BUILD )

include_directories ( 

  ${MOAIUNTZ_INCLUDES} 
  ${MOAICORE_INCLUDES} 
  ${THIRD_PARTY_INCLUDES}
  ${UNTZ_INCLUDES}
  
)

add_library ( moai-untz STATIC 

    ${CMAKE_CURRENT_SOURCE_DIR}/MOAIUntzSampleBuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MOAIUntzSound.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MOAIUntzSystem.cpp
    
)