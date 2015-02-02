cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-http-server )


set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-http-server/" )


set ( MOAI_HTTP_SERVER_INCLUDES 
  "${MOAI_ROOT}/src/"
  "${CMAKE_BINARY_DIR}/libmoai/config"
)

include_directories ( 
  ${MOAI_HTTP_SERVER_INCLUDES} 
)


# Use all cpp files in directory as source code
file ( GLOB MOAI_HTTP_SERVER_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

add_library ( moai-http-server STATIC ${MOAI_HTTP_SERVER_SRC} )
target_link_libraries ( moai-http-server moai-core moai-util  )

target_include_directories (moai-http-server PUBLIC 
    "$<BUILD_INTERFACE:${MOAI_HTTP_SERVER_INCLUDES}>"
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/moai-http-server>"
)


target_link_libraries ( moai-http-server mongoose )

set_target_properties(moai-http-server PROPERTIES FOLDER Modules)  
install(TARGETS moai-http-server EXPORT libmoai ARCHIVE DESTINATION lib)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include/moai-http-server
          FILES_MATCHING PATTERN "*.h")


  

