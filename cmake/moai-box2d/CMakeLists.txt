cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-box2d )


  set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-box2d/" )
 

  set ( MOAI_BOX2D_INCLUDES 
    "${MOAI_ROOT}/src/"
    "${MOAI_ROOT}/src/config-default/"
    )

  # Use all cpp files in directory as source code
  file ( GLOB MOAI_BOX2D_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

  include_directories ( ${MOAI_BOX2D_INCLUDES} )

  add_library ( moai-box2d STATIC ${MOAI_BOX2D_SRC} )

  get_target_property (BOX2D_INCLUDES box2d INCLUDE_DIRECTORIES)
  target_include_directories (moai-box2d PUBLIC ${MOAI_BOX2D_INCLUDES} ${BOX2D_INCLUDES})

  target_link_libraries ( moai-box2d moai-sim moai-core moai-util box2d zlcore )
  set_target_properties(moai-box2d PROPERTIES FOLDER Modules)  
