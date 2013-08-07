cmake_minimum_required ( VERSION 2.8.5 )

project ( expat )

if ( MOAI_EXPAT )

  set ( CMAKE_CURRENT_SOURCE_DIR "../../3rdparty/expat-2.0.1/lib/" )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_EXPAT=1" CACHE INTERNAL "doc string")
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_EXPAT=1" CACHE INTERNAL "doc string")

  if ( BUILD_WINDOWS )
    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_MEMMOVE=1 -DXML_DTD" CACHE INTERNAL "doc string")
  endif (BUILD_WINDOWS)

  set ( EXPAT_INCLUDES "../../3rdparty/expat-2.0.1/lib/" CACHE INTERNAL "doc string" )

  set ( EXPAT_SOURCES 

    ${CMAKE_CURRENT_SOURCE_DIR}/xmlparse.c
    ${CMAKE_CURRENT_SOURCE_DIR}/xmlrole.c
    ${CMAKE_CURRENT_SOURCE_DIR}/xmltok.c
    ${CMAKE_CURRENT_SOURCE_DIR}/xmltok_impl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/xmltok_ns.c
      
  CACHE INTERNAL "doc string" )

else ( MOAI_EXPAT )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_EXPAT=0" CACHE INTERNAL "doc string")
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_EXPAT=0" CACHE INTERNAL "doc string")

endif ( MOAI_EXPAT )
