cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-apple )

if ( BUILD_ANDROID )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -include ${MOAI_ROOT}/src/zl-vfs/zl_replace.h" )
   set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include ${MOAI_ROOT}/src/zl-vfs/zl_replace.h" )
endif ( BUILD_ANDROID )

set ( MOAI_APPLE_INCLUDES 
  "${MOAI_ROOT}/src/"
  "${CMAKE_BINARY_DIR}/libmoai/config"
  "${MOAI_ROOT}/src/moai-apple/"
  "${MOAI_ROOT}/3rdparty/"
  )

include_directories ( 
  ${MOAI_APPLE_INCLUDES} 
)

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-apple/" )

file ( GLOB MOAI_APPLE_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/*.m"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.mm"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${MOAI_ROOT}/3rdparty/contrib/MOAIIso8601DateFormatter.m"
    "${MOAI_ROOT}/3rdparty/contrib/MOAIIso8601DateFormatter.h"
    "${MOAI_ROOT}/3rdparty/contrib/MOAIOpenUDID.m"
    "${MOAI_ROOT}/3rdparty/contrib/MOAIOpenUDID.h"
)

include_directories ( ${MOAI_APPLE_INCLUDES} )

add_library ( moai-apple STATIC ${MOAI_APPLE_SRC} )

target_link_libraries ( moai-apple moai-util moai-core  )


target_include_directories (moai-apple PUBLIC 
    "$<BUILD_INTERFACE:${MOAI_APPLE_INCLUDES}>"
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/moai-apple>"
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/contrib>"
)

set_target_properties( moai-apple PROPERTIES FOLDER Core)

install(TARGETS moai-apple EXPORT libmoai ARCHIVE DESTINATION lib)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include/moai-apple
          FILES_MATCHING PATTERN "*.h")

