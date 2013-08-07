cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-box2d )

if ( MOAI_BOX2D )
  set ( CMAKE_CURRENT_SOURCE_DIR "../../src/moai-box2d/" )

  set ( MOAI_BOX2D_INCLUDES 
    ${THIRD_PARTY_INCLUDES}
    "../../src/"
    "../../src/config-default/"

   CACHE INTERNAL "doc string" )

  # Use all cpp files in directory as source code
  file ( GLOB MOAI_BOX2D_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )

  include_directories ( ${MOAI_BOX2D_INCLUDES} )

  add_library ( moai-box2d STATIC ${MOAI_BOX2D_SRC} )
  target_link_libraries ( moai-box2d third-party)

endif ( MOAI_BOX2D )
