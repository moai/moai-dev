cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-crypto )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-crypto/" )

set ( MOAI_CRYPTO_INCLUDES 
  "${MOAI_ROOT}/src/"
  "${MOAI_ROOT}/src/config-default/"
)

# Use all cpp files in directory as source code
file ( GLOB MOAI_CRYPTO_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )


add_library ( moai-crypto STATIC ${MOAI_CRYPTO_SRC} )


#target_include_directories( moai-crypto PUBLIC ${MOAI_CRYPTO_INCLUDES} )
target_include_directories ( moai-crypto PUBLIC 
  "$<BUILD_INTERFACE:${MOAI_CRYPTO_INCLUDES}>"
  $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
)


target_link_libraries ( moai-crypto moai-sim moai-util moai-core zlcrypto zlcore  )
  
set_target_properties( moai-crypto PROPERTIES FOLDER Modules )  

install(TARGETS moai-crypto EXPORT libmoai ARCHIVE DESTINATION lib)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include/moai-crypto
          FILES_MATCHING PATTERN "*.h")
