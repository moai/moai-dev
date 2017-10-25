cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-sim )


set ( MOAI_SIM_INCLUDES 
  "${MOAI_ROOT}/src/"
  "${CMAKE_BINARY_DIR}/libmoai/config"

  )

include_directories ( 
  ${MOAI_SIM_INCLUDES} 
)

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-sim/" )

# Use all cpp files in directory as source code
file ( GLOB MOAI_SIM_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

include_directories ( ${MOAI_SIM_INCLUDES} )

add_library ( moai-sim STATIC ${MOAI_SIM_SRC} )

target_link_libraries ( moai-sim moai-core moai-util contrib zlcore tess)

#always point to freetype includes so the generated project is easy to turn freetype back on!
get_target_property ( FREETYPE_INCLUDES freetype INCLUDE_DIRECTORIES )

target_include_directories (moai-sim PUBLIC 
    "$<BUILD_INTERFACE:${MOAI_SIM_INCLUDES} ${FREETYPE_INCLUDES}>"
     $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
     $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/freetype>
)


set_target_properties(moai-sim PROPERTIES FOLDER Core)

if (MOAI_FREETYPE)
	target_link_libraries( moai-sim freetype)
endif (MOAI_FREETYPE)



if (MOAI_PNG)
	target_link_libraries( moai-sim png)
endif(MOAI_PNG)

if (MOAI_JPG)
	target_link_libraries (moai-sim jpg)
endif (MOAI_JPG)

if (MOAI_WEBP)
	target_link_libraries (moai-sim webp)
endif (MOAI_WEBP)

if (MOAI_PVR)
  target_link_libraries (moai-sim pvr)
endif (MOAI_PVR)

if (MOAI_BOX2D)
 target_link_libraries ( moai-sim moai-box2d)
endif (MOAI_BOX2D)

if (BUILD_WINDOWS)
 target_link_libraries ( moai-sim  psapi rpcrt4 ) #GetProcessMemoryInfo and UuidCreateSequential
endif (BUILD_WINDOWS)

install(TARGETS moai-sim EXPORT libmoai ARCHIVE DESTINATION lib)



install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include/moai-sim
          FILES_MATCHING PATTERN "*.h")
