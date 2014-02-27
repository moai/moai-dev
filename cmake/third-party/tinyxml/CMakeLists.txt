cmake_minimum_required ( VERSION 2.8.5 )
project ( tinyxml )

  set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/tinyxml/" )

  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTIXML_USE_STL"  )

  set ( TINYXML_INCLUDES "${MOAI_ROOT}/3rdparty/tinyxml/" )

  set ( TINYXML_SOURCES

    ${CMAKE_CURRENT_SOURCE_DIR}/tinystr.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tinyxml.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tinyxmlerror.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tinyxmlparser.cpp


   )

 add_library (tinyxml STATIC ${TINYXML_SOURCES} )
 target_include_directories ( tinyxml PUBLIC ${TINYXML_INCLUDES})
 set_target_properties( tinyxml PROPERTIES FOLDER Third-Party )