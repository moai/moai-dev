cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-util )

if ( BUILD_ANDROID )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -include ${MOAI_ROOT}/src/zl-vfs/zl_replace.h" )
   set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include ${MOAI_ROOT}/src/zl-vfs/zl_replace.h" )
endif ( BUILD_ANDROID )

set ( MOAI_UTIL_INCLUDES 
  "${MOAI_ROOT}/src/"
  "${CMAKE_BINARY_DIR}/libmoai/config"
  )

include_directories ( 
  ${MOAI_UTIL_INCLUDES} 
   ${LUA_HEADERS}
)

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-util/" )

file ( GLOB MOAI_UTIL_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )



add_library ( moai-util STATIC ${MOAI_UTIL_SRC} )

target_link_libraries ( moai-util moai-core zlcore kissfft ${LUA_LIB} )


target_include_directories (moai-util PUBLIC 
    "$<BUILD_INTERFACE:${MOAI_UTIL_INCLUDES}>"
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/moai-util>"
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/expat>"
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/tinyxml>"
)

set_target_properties( moai-util PROPERTIES FOLDER Core)

if (MOAI_SFMT)
	target_link_libraries (moai-util sfmt )
endif (MOAI_SFMT)

if ( MOAI_JSON )
	target_link_libraries( moai-util jansson )
endif (MOAI_JSON ) 

if ( MOAI_TINYXML )
   target_link_libraries( moai-util tinyxml )
endif ( MOAI_TINYXML )

if (MOAI_OPENSSL)
  target_link_libraries( moai-util ssl )
endif (MOAI_OPENSSL)

install(TARGETS moai-util EXPORT libmoai ARCHIVE DESTINATION lib)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include/moai-util
          FILES_MATCHING PATTERN "*.h")

