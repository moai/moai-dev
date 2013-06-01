cmake_minimum_required ( VERSION 2.8.5 )
project ( tinyxml )

if ( MOAI_TINYXML )

  set ( CMAKE_CURRENT_SOURCE_DIR "../../3rdparty/tinyxml/" )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_TINYXML=1" CACHE INTERNAL "doc string" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_TINYXML=1" CACHE INTERNAL "doc string" )

  set ( TINYXML_INCLUDES "../../3rdparty/tinyxml/" CACHE INTERNAL "doc string" )

  set ( TINYXML_SOURCES 

    ${CMAKE_CURRENT_SOURCE_DIR}/tinystr.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tinyxml.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tinyxmlerror.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tinyxmlparser.cpp


  CACHE INTERNAL "doc string" )

else ( MOAI_TINYXML )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_TINYXML=0" CACHE INTERNAL "doc string" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_TINYXML=0" CACHE INTERNAL "doc string" )

endif ( MOAI_TINYXML )
