cmake_minimum_required ( VERSION 2.8.5 )

project ( contrib )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/contrib/" )

set ( CONTRIB_INCLUDES
  "${MOAI_ROOT}/3rdparty/"
)

set ( CONTRIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/utf8.c
    ${CMAKE_CURRENT_SOURCE_DIR}/whirlpool.c
)

include_directories(${CONTRIB_INCLUDES})
add_library( contrib STATIC ${CONTRIB_SOURCES})
target_include_directories (contrib PUBLIC ${CONTRIB_INCLUDES})

