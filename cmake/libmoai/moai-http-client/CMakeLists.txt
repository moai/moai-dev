cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-http-client )


set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-http-client/" )


set ( MOAI_HTTP_CLIENT_INCLUDES 
  "${MOAI_ROOT}/src/"
  "${CMAKE_BINARY_DIR}/libmoai/config"
)

include_directories ( 
  ${MOAI_HTTP_CLIENT_INCLUDES} 
)


# Use all cpp files in directory as source code
file ( GLOB MOAI_HTTP_CLIENT_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

if (BUILD_IOS OR BUILD_OSX)
  set (MOAI_HTTP_CLIENT_SRC 
  	${MOAI_HTTP_CLIENT_SRC}  
  	"${CMAKE_CURRENT_SOURCE_DIR}/MOAIHttpTaskNSURL.mm" 
  )	
endif (BUILD_IOS OR BUILD_OSX)

add_library ( moai-http-client STATIC ${MOAI_HTTP_CLIENT_SRC} )
target_link_libraries ( moai-http-client moai-core moai-util  )

target_include_directories (moai-http-client PUBLIC 
    "$<BUILD_INTERFACE:${MOAI_HTTP_CLIENT_INCLUDES}>"
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/moai-http-client>"
)

if ( MOAI_CURL )
 target_link_libraries ( moai-http-client curl )
endif (MOAI_CURL)

set_target_properties(moai-http-client PROPERTIES FOLDER Modules)  
install(TARGETS moai-http-client EXPORT libmoai ARCHIVE DESTINATION lib)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include/moai-http-client
          FILES_MATCHING PATTERN "*.h")


  

