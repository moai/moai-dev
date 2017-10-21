cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-image-jpg )

set (MODULE_NAME "moai-image-jpg")
set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/${MODULE_NAME}/" )
get_target_property (MODULE_EXTRA_INCLUDES jpg INCLUDE_DIRECTORIES)
set (MODULE_EXTRA_LIBS "jpg")


  set ( MOAI_MODULE_INCLUDES 
    "${MOAI_ROOT}/src/"
    "${CMAKE_BINARY_DIR}/libmoai/config"
    )

  # Use all cpp files in directory as source code
  file ( GLOB MOAI_MODULE_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

  include_directories ( ${MOAI_MODULE_INCLUDES} )

  add_library ( ${MODULE_NAME} STATIC ${MOAI_MODULE_SRC} )



  set(BUILD_INCLUDES ${MOAI_MODULE_INCLUDES} ${MODULE_EXTRA_INCLUDES})
 

  target_include_directories (${MODULE_NAME} PUBLIC 
        "$<BUILD_INTERFACE:${BUILD_INCLUDES}>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
       
  )

  target_link_libraries ( ${MODULE_NAME} moai-sim moai-core moai-util ${MODULE_EXTRA_LIBS} zlcore )
  set_target_properties(${MODULE_NAME} PROPERTIES FOLDER Modules)  

  install(TARGETS ${MODULE_NAME} EXPORT libmoai ARCHIVE DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include/${MODULE_NAME}
          FILES_MATCHING PATTERN "*.h")
  
