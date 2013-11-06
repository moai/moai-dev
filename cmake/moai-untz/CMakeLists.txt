cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-untz )

if ( MOAI_UNTZ )

  set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-untz/" )
  set ( MOAI_UNTZ_INCLUDES "${MOAI_ROOT}/src/;${MOAI_ROOT}/src/moai-untz/" CACHE INTERNAL "doc string" )

  if ( FLASCC_BUILD )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_CURL=false -DUSE_OPENSSL=false")
    include_directories ( "${MOAI_ROOT}/3rdparty/GLS3D/install/usr/include/")
    include_directories ( "${FLASCC}/usr/include/SDL")
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__FLASCC__ -DPOSIX -D__SDL__" )
    
  endif ( FLASCC_BUILD )

  include_directories ( 
    ${THIRD_PARTY_INCLUDES}
    ${MOAI_UNTZ_INCLUDES} 
    ${MOAI_CORE_INCLUDES} 
    ${UNTZ_INCLUDES}
  )

  add_library ( moai-untz STATIC 
      ${CMAKE_CURRENT_SOURCE_DIR}/host.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/MOAIUntzSampleBuffer.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/MOAIUntzSound.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/MOAIUntzSystem.cpp
  )

endif ( MOAI_UNTZ )
