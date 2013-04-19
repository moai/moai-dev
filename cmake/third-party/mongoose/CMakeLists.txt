cmake_minimum_required ( VERSION 2.8.5 )
project ( mongoose )


set ( CMAKE_CURRENT_SOURCE_DIR "../../../3rdparty/mongoose/" )

set ( MONGOOSE_INCLUDES "../../3rdparty/mongoose/" CACHE INTERNAL "doc string" )

set ( MONGOOSE_SOURCES 

  ${CMAKE_CURRENT_SOURCE_DIR}/mongoose.c
  
CACHE INTERNAL "doc string" )