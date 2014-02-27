cmake_minimum_required ( VERSION 2.8.5 )
project ( jansson )

  set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/jansson-2.1/src/" )



  set ( JANSSON_INCLUDES "${MOAI_ROOT}/3rdparty/jansson-2.1/src/" )

  if (BUILD_WINDOWS AND MSVC)
    #need a platform header for this. snprintf doesn't exist
    set ( JANSSON_INCLUDES "${MOAI_ROOT}/3rdparty/jansson-2.1/src-win32" ${JANSSON_INCLUDES})
  endif( BUILD_WINDOWS AND MSVC)

  set ( JANSSON_SOURCES 

      ${CMAKE_CURRENT_SOURCE_DIR}/dump.c
      ${CMAKE_CURRENT_SOURCE_DIR}/error.c
      ${CMAKE_CURRENT_SOURCE_DIR}/hashtable.c
      ${CMAKE_CURRENT_SOURCE_DIR}/load.c
      ${CMAKE_CURRENT_SOURCE_DIR}/memory.c
      ${CMAKE_CURRENT_SOURCE_DIR}/pack_unpack.c
      ${CMAKE_CURRENT_SOURCE_DIR}/strbuffer.c
      ${CMAKE_CURRENT_SOURCE_DIR}/utf.c
      ${CMAKE_CURRENT_SOURCE_DIR}/value.c
      
  )
  
  add_library(jansson STATIC ${JANSSON_SOURCES})
  target_include_directories(jansson PUBLIC ${JANSSON_INCLUDES})
  set_target_properties( jansson PROPERTIES FOLDER Third-Party )