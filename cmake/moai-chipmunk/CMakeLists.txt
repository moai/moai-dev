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

set(BUILD_INCLUDES ${MOAI_CHIPMUNK_INCLUDES} ${CHIPMUNK_INCLUDES})
 
target_include_directories (moai-chipmunk PUBLIC 
    "$<BUILD_INTERFACE:${BUILD_INCLUDES}>"
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/moai-chipmunk>"
   
)

target_link_libraries ( moai-chipmunk moai-sim chipmunk moai-util moai-core zlcore )
set_target_properties(moai-chipmunk PROPERTIES FOLDER Modules)  
install(TARGETS moai-chipmunk EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/host.h DESTINATION include/moai-chipmunk)