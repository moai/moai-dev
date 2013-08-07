cmake_minimum_required ( VERSION 2.8.5 )
project ( mongoose )

if ( MOAI_MONGOOSE )

  set ( CMAKE_CURRENT_SOURCE_DIR "../../3rdparty/mongoose/" )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_MONGOOSE=1" CACHE INTERNAL "doc string")
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_MONGOOSE=1" CACHE INTERNAL "doc string")

  if ( BUILD_WINDOWS )
 	set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_STDINT=1" CACHE INTERNAL "doc string")
  

  endif ( BUILD_WINDOWS )

  set ( MONGOOSE_INCLUDES "../../3rdparty/mongoose/" CACHE INTERNAL "doc string" )

  set ( MONGOOSE_SOURCES 

    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose.c
    
  CACHE INTERNAL "doc string" )

else ( MOAI_MONGOOSE )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_MONGOOSE=0" CACHE INTERNAL "doc string")
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_MONGOOSE=0" CACHE INTERNAL "doc string")

endif ( MOAI_MONGOOSE )
