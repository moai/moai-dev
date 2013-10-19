cmake_minimum_required ( VERSION 2.8.5 )
project ( tess2 )

if ( FVH_TESS2 )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_FVH_TESS2=1" CACHE INTERNAL "doc string")
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_FVH_TESS2=1" CACHE INTERNAL "doc string")

  set ( CMAKE_CURRENT_SOURCE_DIR "../../../3rdparty/libtess2/Source/" )

  set ( TESS2_INCLUDES "../../3rdparty/libtess2/Include/" CACHE INTERNAL "doc string" )

  set ( TESS2_SOURCES 

    ${CMAKE_CURRENT_SOURCE_DIR}/bucketalloc.c   
    ${CMAKE_CURRENT_SOURCE_DIR}/dict.c
    ${CMAKE_CURRENT_SOURCE_DIR}/geom.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mesh.c
    ${CMAKE_CURRENT_SOURCE_DIR}/priorityq.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sweep.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tess.c 

  CACHE INTERNAL "doc string" )

endif ( FVH_TESS2 )
