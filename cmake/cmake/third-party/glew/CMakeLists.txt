cmake_minimum_required ( VERSION 2.8.5 )
project ( glew )


set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -DGLEW_STATIC " )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGLEW_STATIC" )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/glew-1.5.6/src" )



set ( GLEW_INCLUDES "${MOAI_ROOT}/3rdparty/glew-1.5.6/include"  )

set ( GLEW_SOURCES

    ${CMAKE_CURRENT_SOURCE_DIR}/glew.c

 )

add_library ( glew STATIC ${GLEW_SOURCES} )
target_include_directories ( glew PUBLIC "$<BUILD_INTERFACE:${GLEW_INCLUDES}>" )
target_link_libraries ( glew  ${OPENGL_LIBRARIES})
set_target_properties( glew PROPERTIES FOLDER Third-Party )
install(TARGETS glew EXPORT libmoai ARCHIVE DESTINATION lib)