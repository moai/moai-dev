cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-spine )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-spine/" )

set ( MOAI_SPINE_INCLUDES 
  "${MOAI_ROOT}/src/"
  "${CMAKE_BINARY_DIR}/libmoai/config"
)

# Use all cpp files in directory as source code
file ( GLOB MOAI_SPINE_SRC 
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" 
  "${CMAKE_CURRENT_SOURCE_DIR}/*.h" 
)

include_directories ( ${MOAI_SPINE_INCLUDES} )

add_library ( moai-spine STATIC ${MOAI_SPINE_SRC} )

get_target_property (SPINE_INCLUDES spine INCLUDE_DIRECTORIES)

set(BUILD_INCLUDES ${MOAI_SPINE_INCLUDES} ${SPINE_INCLUDES})


target_include_directories (moai-spine PUBLIC 
      "$<BUILD_INTERFACE:${BUILD_INCLUDES}>"
      "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
     
)

target_link_libraries ( moai-spine moai-sim moai-core moai-util spine zlcore )
set_target_properties(moai-spine PROPERTIES FOLDER Modules)  

install(TARGETS moai-spine EXPORT libmoai ARCHIVE DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include/moai-spine
          FILES_MATCHING PATTERN "*.h")
