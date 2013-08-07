cmake_minimum_required ( VERSION 2.8.5 )
project ( glew )

if ( NOT BUILD_BLACKBERRY )

set ( CMAKE_CURRENT_SOURCE_DIR "../../3rdparty/glew-1.5.6/src" )

set ( GLEW_INCLUDES "../../3rdparty/glew-1.5.6/include" CACHE INTERNAL "doc string" )

set ( GLEW_SOURCES 

    ${CMAKE_CURRENT_SOURCE_DIR}/glew.c
    
CACHE INTERNAL "doc string" )

endif ( NOT BUILD_BLACKBERRY )