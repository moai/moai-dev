cmake_minimum_required ( VERSION 2.8.5 )
project ( glew )

if ( NOT BUILD_BLACKBERRY )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/glew-1.5.6/src" )

set ( GLEW_INCLUDES "${MOAI_ROOT}/3rdparty/glew-1.5.6/include" PARENT_SCOPE )

set ( GLEW_SOURCES

    ${CMAKE_CURRENT_SOURCE_DIR}/glew.c

PARENT_SCOPE )

endif ( NOT BUILD_BLACKBERRY )