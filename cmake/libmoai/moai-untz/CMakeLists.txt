cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-untz )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-untz/" )
set ( MOAI_UNTZ_INCLUDES "${MOAI_ROOT}/src/;${CMAKE_BINARY_DIR}/libmoai/config;${MOAI_ROOT}/src/moai-untz/"  )

if ( FLASCC_BUILD )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_CURL=false -DUSE_OPENSSL=false")
  include_directories ( "${MOAI_ROOT}/3rdparty/GLS3D/install/usr/include/")
  include_directories ( "${FLASCC}/usr/include/SDL")
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__FLASCC__ -DPOSIX -D__SDL__" )
  
endif ( FLASCC_BUILD )



include_directories ( 
   ${MOAI_UNTZ_INCLUDES}
)

# Use all cpp files in directory as source code
file ( GLOB MOAI_UNTZ_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

add_library ( moai-untz STATIC ${MOAI_UNTZ_SRC} )

target_link_libraries (moai-untz moai-sim moai-core moai-util untz)

target_include_directories (moai-untz PUBLIC 
    "$<BUILD_INTERFACE:${MOAI_UNTZ_INCLUDES}>"
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/untz>"
)

set_target_properties(moai-untz PROPERTIES FOLDER Modules)  
install(TARGETS moai-untz EXPORT libmoai ARCHIVE DESTINATION lib)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include/moai-untz
          FILES_MATCHING PATTERN "*.h")