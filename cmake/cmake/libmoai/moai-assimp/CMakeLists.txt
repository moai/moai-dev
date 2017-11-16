cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-assimp )


  set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-assimp/" )
 

  set ( MOAI_ASSIMP_INCLUDES 
    "${MOAI_ROOT}/src/"
    "${CMAKE_BINARY_DIR}/libmoai/config"
    )

  # Use all cpp files in directory as source code
  file ( GLOB MOAI_ASSIMP_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

  include_directories ( ${MOAI_ASSIMP_INCLUDES} )

  add_library ( moai-assimp STATIC ${MOAI_ASSIMP_SRC} )


  set(BUILD_INCLUDES ${MOAI_ASSIMP_INCLUDES})
 

  target_include_directories (moai-assimp PUBLIC 
        "$<BUILD_INTERFACE:${BUILD_INCLUDES}>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
       
  )

  target_link_libraries ( moai-assimp moai-sim moai-core moai-util assimp zlcore )
  set_target_properties(moai-assimp PROPERTIES FOLDER Modules)  

  install(TARGETS moai-assimp EXPORT libmoai ARCHIVE DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include/moai-assimp
          FILES_MATCHING PATTERN "*.h")
  
