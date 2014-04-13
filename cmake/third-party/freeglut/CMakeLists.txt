cmake_minimum_required ( VERSION 2.8.8 )
project ( freeglut )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/freeglut-2.8.1/src" )
set ( FREEGLUT_INCLUDES
  "${MOAI_ROOT}/3rdparty/freeglut-2.8.1/include/GL"
  "${MOAI_ROOT}/3rdparty/freeglut-2.8.1/include"
)
if (MSVC)
	set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /DFREEGLUT_STATIC /DFREEGLUT_LIB_PRAGMAS=0" )
	set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DFREEGLUT_STATIC /DFREEGLUT_LIB_PRAGMAS=0" )
else (MSVC)
	set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DFREEGLUT_STATIC -DFREEGLUT_LIB_PRAGMAS=0" )
	set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFREEGLUT_STATIC -DFREEGLUT_LIB_PRAGMAS=0" )
endif (MSVC)
include_directories ( ${FREEGLUT_INCLUDES} )

file ( GLOB FREEGLUT_SOURCES "${MOAI_ROOT}/3rdparty/freeglut-2.8.1/src/*.c" )
file ( GLOB FREEGLUT_HEADERS "${MOAI_ROOT}/3rdparty/freeglut-2.8.1/src/*.h" )

add_library ( freeglut_static STATIC ${FREEGLUT_SOURCES} ${FREEGLUT_HEADERS} )
target_include_directories (freeglut_static PUBLIC 
   "$<BUILD_INTERFACE:${FREEGLUT_INCLUDES}>"
   "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/freeglut>"
)

find_package( OpenGL REQUIRED )

target_link_libraries( freeglut_static ${OPENGL_LIBRARIES})

set_target_properties( freeglut_static PROPERTIES FOLDER Third-Party )

if (BUILD_LINUX)
  target_link_libraries( freeglut_static Xxf86vm )
endif (BUILD_LINUX)

install(TARGETS freeglut_static EXPORT libmoai ARCHIVE DESTINATION lib)

install(DIRECTORY ${MOAI_ROOT}/3rdparty/freeglut-2.8.1/include/
        DESTINATION include/freeglut)