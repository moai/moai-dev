cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-chipmunk )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-chipmunk/" )


set ( MOAI_CHIPMUNK_INCLUDES 
  "${MOAI_ROOT}/src/"
  "${MOAI_ROOT}/src/config-default/"
)

# Use all cpp files in directory as source code
file ( GLOB MOAI_CHIPMUNK_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

include_directories ( ${MOAI_CHIPMUNK_INCLUDES} )

add_library ( moai-chipmunk STATIC ${MOAI_CHIPMUNK_SRC} )

get_target_property( CHIPMUNK_INCLUDES chipmunk INCLUDE_DIRECTORIES )
target_include_directories( moai-chipmunk PUBLIC ${MOAI_CHIPMUNK_INCLUDES} ${CHIPMUNK_INCLUDES} )

target_link_libraries ( moai-chipmunk moai-sim chipmunk moai-util moai-core zlcore )
set_target_properties(moai-chipmunk PROPERTIES FOLDER Modules)  