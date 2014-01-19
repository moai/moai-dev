cmake_minimum_required ( VERSION 2.8.5 )

project ( expat )



  set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/expat-2.0.1/lib/" )


  if ( BUILD_WINDOWS )
    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_MEMMOVE=1 -DXML_DTD" )
  endif (BUILD_WINDOWS)
  
  if ( BUILD_ANDROID ) 
  	set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_MEMMOVE=1 -DXML_STATIC" )
  endif (BUILD_ANDROID )

  set ( EXPAT_INCLUDES "${MOAI_ROOT}/3rdparty/expat-2.0.1/lib/" )

  set ( EXPAT_SOURCES 

    ${CMAKE_CURRENT_SOURCE_DIR}/xmlparse.c
    ${CMAKE_CURRENT_SOURCE_DIR}/xmlrole.c
    ${CMAKE_CURRENT_SOURCE_DIR}/xmltok.c
    ${CMAKE_CURRENT_SOURCE_DIR}/xmltok_impl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/xmltok_ns.c
      
   )

   add_library (expat STATIC ${EXPAT_SOURCES})
   target_include_directories (expat PUBLIC ${EXPAT_INCLUDES})
