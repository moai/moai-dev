cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-harfbuzz )


  set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-harfbuzz/" )
 

  set ( MOAI_HARFBUZZ_INCLUDES 
    "${MOAI_ROOT}/src/"
    "${CMAKE_BINARY_DIR}/libmoai/config"
    )

  # Use all cpp files in directory as source code
  file ( GLOB MOAI_HARFBUZZ_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

  include_directories ( ${MOAI_HARFBUZZ_INCLUDES} )

  add_library ( moai-harfbuzz STATIC ${MOAI_HARFBUZZ_SRC} )


  set(BUILD_INCLUDES ${MOAI_HARFBUZZ_INCLUDES})
 

  target_include_directories (moai-harfbuzz PUBLIC 
        "$<BUILD_INTERFACE:${BUILD_INCLUDES}>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
       
  )

  target_link_libraries ( moai-harfbuzz moai-sim moai-core moai-util harfbuzz zlcore )
  set_target_properties(moai-harfbuzz PROPERTIES FOLDER Modules)  

  install(TARGETS moai-harfbuzz EXPORT libmoai ARCHIVE DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include/moai-harfbuzz
          FILES_MATCHING PATTERN "*.h")
  
