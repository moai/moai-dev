cmake_minimum_required ( VERSION 2.8.5 )
project ( sqlite3 )

  set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/sqlite-3.6.16/" )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -DSQLITE_ENABLE_COLUMN_METADATA -Dfdatasync=fsync" )
  
  set ( SQLITE3_INCLUDES "${MOAI_ROOT}/3rdparty/sqlite-3.6.16/"  )

  set ( SQLITE3_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/sqlite3.c
   )

  add_library ( sqlite3 STATIC ${SQLITE3_SOURCES} )

  target_include_directories ( sqlite3 PUBLIC ${SQLITE3_INCLUDES} )