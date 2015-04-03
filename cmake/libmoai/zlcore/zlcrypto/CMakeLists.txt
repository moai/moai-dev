cmake_minimum_required ( VERSION 2.8.5 )
project ( zlcrypto )


# Define source directory
set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/" )

# Define needed include files
set ( ZLCRYPTO_INCLUDES
 "${CMAKE_BINARY_DIR}/libmoai/config"
  "${MOAI_ROOT}/src/"
 )

# Use all cpp files in directory as source code
file ( GLOB ZLCRYPTO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/zl-crypto/*.cpp" )
file ( GLOB ZLCRYPTO_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/zl-crypto/*.h" )





add_library ( zlcrypto STATIC ${ZLCRYPTO_SOURCES} ${ZLCRYPTO_HEADERS} )


set_target_properties(zlcrypto PROPERTIES FOLDER Core)


target_include_directories(zlcrypto PRIVATE ${ZLCRYPTO_INCLUDES})
target_link_libraries( zlcrypto crypto zlcore)



install(TARGETS zlcrypto EXPORT libmoai ARCHIVE DESTINATION lib)


install(FILES ${ZLCRYPTO_HEADERS} DESTINATION include/zl-crypto)
