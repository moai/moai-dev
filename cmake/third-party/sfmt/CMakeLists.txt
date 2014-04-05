cmake_minimum_required ( VERSION 2.8.5 )
project ( sfmt )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/sfmt-1.4/" )

set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSFMT_MEXP=19937" )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSFMT_MEXP=19937" )

set ( SFMT_INCLUDES "${MOAI_ROOT}/3rdparty/sfmt-1.4/"  )

set ( SFMT_MEXP 19937 )
set ( SFMT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/SFMT.c )
set ( SFMT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/SFMT.h )
include_directories(${SFMT_INCLUDES})

add_library ( sfmt STATIC ${SFMT_SOURCES} ${SFMT_HEADERS} )
target_include_directories ( sfmt PUBLIC ${SFMT_INCLUDES} )
set_target_properties ( sfmt PROPERTIES FOLDER Third-Party )
