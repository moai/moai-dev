cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-http-client )


set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-http-client/" )


set ( MOAI_HTTP_CLIENT_INCLUDES 
  "${MOAI_ROOT}/src/"
  "${MOAI_ROOT}/src/config-default/"
)

include_directories ( 
  ${MOAI_HTTP_CLIENT_INCLUDES} 
)


# Use all cpp files in directory as source code
file ( GLOB MOAI_HTTP_CLIENT_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

if (BUILD_IOS OR BUILD_OSX)
  set (MOAI_HTTP_CLIENT_SRC 
  	${MOAI_HTTP_CLIENT_SRC}  
  	"${MOAI_ROOT}/src/moai-iphone/MOAIHttpTaskNSURL.mm" 
  	"${MOAI_ROOT}/src/moai-iphone/MOAIUrlMgrNSURL.mm" 
  )	
endif (BUILD_IOS OR BUILD_OSX)

add_library ( moai-http-client STATIC ${MOAI_HTTP_CLIENT_SRC} )
target_link_libraries ( moai-http-client moai-core moai-util  )


if ( MOAI_CURL )
 target_link_libraries ( moai-http-client curl )
endif (MOAI_CURL)

set_target_properties(moai-http-client PROPERTIES FOLDER Modules)  


  

