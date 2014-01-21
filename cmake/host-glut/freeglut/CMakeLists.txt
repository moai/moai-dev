cmake_minimum_required ( VERSION 2.8.8 )
project ( freeglut )

set ( FREEGLUT_INCLUDES
  "${MOAI_ROOT}/3rdparty/freeglut-2.8.1/include/GL"
  "${MOAI_ROOT}/3rdparty/freeglut-2.8.1/include"
)
include_directories ( ${FREEGLUT_INCLUDES} )
set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DFREEGLUT_STATIC" )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFREEGLUT_STATIC" )
set ( FREEGLUT_INCLUDES ${FREEGLUT_INCLUDES} )

file ( GLOB FREEGLUT_SRC "${MOAI_ROOT}/3rdparty/freeglut-2.8.1/src/*.c" )

add_library ( freeglut-static STATIC ${FREEGLUT_SRC} )
target_include_directories (freeglut-static PUBLIC ${FREEGLUT_INCLUDES})
target_link_libraries( freeglut-static ${OPENGL_LIBRARIES})
