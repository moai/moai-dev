cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-http-client )

if ( MOAI_HTTP_CLIENT )
set ( CMAKE_CURRENT_SOURCE_DIR "../../src/moai-http-client/" )

set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_HTTP_CLIENT=1" CACHE INTERNAL "doc string" )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_HTTP_CLIENT=1" CACHE INTERNAL "doc string" )

set ( MOAI_HTTP_CLIENT_INCLUDES 
  ${THIRD_PARTY_INCLUDES}
  "../../src/"
  "../../src/config-default/"

 CACHE INTERNAL "doc string" )

include_directories ( 
  ${MOAI_HTTP_CLIENT_INCLUDES} 
)


# Use all cpp files in directory as source code
file ( GLOB MOAI_HTTP_CLIENT_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )

add_library ( moai-http-client STATIC ${MOAI_HTTP_CLIENT_SRC} )

else ( MOAI_HTTP_CLIENT )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_HTTP_CLIENT=0" CACHE INTERNAL "doc string" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_HTTP_CLIENT=0" CACHE INTERNAL "doc string" )

endif ( MOAI_HTTP_CLIENT )
