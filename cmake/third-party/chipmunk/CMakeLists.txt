cmake_minimum_required ( VERSION 2.8.5 )
project ( chipmunk )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/chipmunk-5.3.4/src/" )
set ( CHIPMUNK_INCLUDES "${MOAI_ROOT}/3rdparty/chipmunk-5.3.4/include/chipmunk/;${MOAI_ROOT}/3rdparty/chipmunk-5.3.4/include/")

if (BUILD_WINDOWS)
    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS -DWIN32 -D_DEBUG -D_LIB " )
endif(BUILD_WINDOWS)

file ( GLOB_RECURSE CHIPMUNK_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.c" )
file ( GLOB_RECURSE CHIPMUNK_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CHIPMUNK_INCLUDES}/*.h" )

add_library (chipmunk STATIC ${CHIPMUNK_SOURCES} ${CHIPMUNK_HEADERS} )
set_target_properties ( chipmunk PROPERTIES LINKER_LANGUAGE CXX )
SET_SOURCE_FILES_PROPERTIES ( ${CHIPMUNK_SOURCES} PROPERTIES LANGUAGE CXX )
target_include_directories (chipmunk PUBLIC ${CHIPMUNK_INCLUDES})
set_target_properties( chipmunk PROPERTIES FOLDER Third-Party )