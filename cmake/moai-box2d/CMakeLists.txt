cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-box2d )

if ( MOAI_BOX2D )
  set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-box2d/" )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DAKU_WITH_BOX2D=1 -DMOAI_WITH_BOX2D=1"  PARENT_SCOPE )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAKU_WITH_BOX2D=1 -DMOAI_WITH_BOX2D=1"  PARENT_SCOPE)

  set ( MOAI_BOX2D_INCLUDES 
    ${THIRD_PARTY_INCLUDES}
    "${MOAI_ROOT}/src/"
    "${MOAI_ROOT}/src/config-default/"

    )

  # Use all cpp files in directory as source code
  file ( GLOB MOAI_BOX2D_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )

  include_directories ( ${MOAI_BOX2D_INCLUDES} )

  add_library ( moai-box2d STATIC ${MOAI_BOX2D_SRC} )
  target_link_libraries ( moai-box2d third-party)

else ( MOAI_BOX2D )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DAKU_WITH_BOX2D=0 -DMOAI_WITH_BOX2D=1"   PARENT_SCOPE)
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAKU_WITH_BOX2D=0 -DMOAI_WITH_BOX2D=1"  PARENT_SCOPE)
endif ( MOAI_BOX2D )
