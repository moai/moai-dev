cmake_minimum_required ( VERSION 2.8.5 )
project ( ogg )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/libogg-1.2.2/src/" )



set ( OGG_INCLUDES "${MOAI_ROOT}/3rdparty/libogg-1.2.2/include/"  )

set ( OGG_SOURCES 
  ${CMAKE_CURRENT_SOURCE_DIR}/bitwise.c
  ${CMAKE_CURRENT_SOURCE_DIR}/framing.c
)

include_directories(${OGG_INCLUDES})
add_library(ogg STATIC ${OGG_SOURCES})
target_include_directories(ogg PUBLIC ${OGG_INCLUDES})
set_target_properties( ogg PROPERTIES FOLDER Third-Party )