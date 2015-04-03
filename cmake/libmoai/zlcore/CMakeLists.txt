cmake_minimum_required ( VERSION 2.8.5 )
project ( zlcore )

add_subdirectory ( zlvfs )
add_subdirectory ( zlcrypto )
# Define source directory
set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/" )

# Define needed include files
set ( ZLCORE_INCLUDES
 "${CMAKE_BINARY_DIR}/libmoai/config"
  "${MOAI_ROOT}/src/"
  "${MOAI_ROOT}/3rdparty/"
  "${MOAI_ROOT}/3rdparty/ooid-0.99/"
 )

# Use all cpp files in directory as source code
file ( GLOB ZLUTIL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/zl-util/*.cpp" )
file ( GLOB ZLUTIL_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/zl-util/*.h" )
file ( GLOB ZLGFX_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/zl-gfx/*.cpp" )
file ( GLOB ZLGFX_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/zl-gfx/*.h" )
file ( GLOB ZLCOMMON_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/zl-common/*.h")


if (BUILD_WINDOWS)   
  list(REMOVE_ITEM ZLUTIL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/zl-util/zl_usocket.cpp")
else (BUILD_WINDOWS)
 list(REMOVE_ITEM ZLUTIL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/zl-util/zl_wsocket.cpp")
endif (BUILD_WINDOWS)
 


include_directories ( ${ZLCORE_INCLUDES} )

add_library ( zlcore STATIC ${ZLUTIL_SOURCES} ${ZLUTIL_HEADERS} ${ZLGFX_SOURCES} ${ZLGFX_HEADERS} )

add_dependencies ( zlcore liblua-static)
set (ZLCORE_TARGET_INCLUDES "${MOAI_ROOT}/src/" "${MOAI_ROOT}/3rdparty/expat-2.1.0/lib")
target_include_directories ( zlcore PUBLIC "$<BUILD_INTERFACE:${ZLCORE_TARGET_INCLUDES}>"
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/expat>"
 )

set_target_properties(zlcore PROPERTIES FOLDER Core)

if ( MOAI_EXPAT )
  target_link_libraries( zlcore expat)
endif ( MOAI_EXPAT )



# compulsory li

if (BUILD_LINUX OR BUILD_WINDOWS OR BUILD_OSX)
  target_link_libraries ( zlcore glew )
endif (BUILD_LINUX OR BUILD_WINDOWS OR BUILD_OSX)

target_link_libraries ( zlcore zlvfs zlib )



target_link_libraries( zlcore ${OPENGL_LIBRARIES})

if (BUILD_WINDOWS)
  target_link_libraries ( zlcore iphlpapi) #GetAdaptersAddresses
endif (BUILD_WINDOWS)


install(TARGETS zlcore EXPORT libmoai ARCHIVE DESTINATION lib)

install(FILES ${ZLUTIL_HEADERS} DESTINATION include/zl-util)
install(FILES ${ZLGFX_HEADERS} DESTINATION include/zl-gfx)
install(FILES ${ZLCOMMON_HEADERS} DESTINATION include/zl-common)

