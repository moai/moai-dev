macro(ANDROID_MODULE MODULE_NAME MODULE_EXTERNAL)
  #tell our parent to dump our setting for ndk_build
  set ( MOAI_ANDROID_INCLUDES 
    "${MOAI_ROOT}/src/"
    "${CMAKE_BINARY_DIR}/libmoai/config"
   )

  
  set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/${MODULE_NAME}" )

  file ( GLOB MOAI_ANDROID_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

  file ( GLOB MOAI_ANDROID_JAVA "${CMAKE_CURRENT_SOURCE_DIR}/*.java")

  if ( MOAI_ANDROID_SRC )

    include_directories ( 
      ${MOAI_ANDROID_INCLUDES} 
    )

    
    #tell our parent to dump our setting for ndk_build
    list(APPEND MOAI_ANDROID_MODULES ${MODULE_NAME})
    set(MOAI_ANDROID_MODULES "${MOAI_ANDROID_MODULES}" PARENT_SCOPE)

    add_library ( ${MODULE_NAME} STATIC ${MOAI_ANDROID_SRC} )
    target_link_libraries( ${MODULE_NAME} moai-sim moai-util moai-core )

    install(TARGETS ${MODULE_NAME} EXPORT libmoai ARCHIVE DESTINATION lib)
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION include/${MODULE_NAME}
              FILES_MATCHING PATTERN "*.h")

  endif ( MOAI_ANDROID_SRC )

  #java
  if (MOAI_ANDROID_JAVA)
    install(FILES ${MOAI_ANDROID_JAVA} DESTINATION ../modules/moai/${MODULE_NAME}/com/ziplinegames/moai)
  endif (MOAI_ANDROID_JAVA)
  
  #third-party
  if (NOT "${MODULE_EXTERNAL}" EQUAL "")
    install(DIRECTORY ${MOAI_ROOT}/3rdparty-android/${MODULE_EXTERNAL}/ DESTINATION ../modules/external/${MODULE_NAME})
  endif ()
  
endmacro(ANDROID_MODULE)