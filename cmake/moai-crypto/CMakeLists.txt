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


target_include_directories( moai-crypto PUBLIC ${MOAI_CRYPTO_INCLUDES} )

target_link_libraries ( moai-crypto moai-sim moai-util moai-core zlcore )
  
set_target_properties( moai-crypto PROPERTIES FOLDER Modules )  
