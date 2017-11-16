cmake_minimum_required ( VERSION 2.8.5 )
project ( zlvfs )

# Define source directory
set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/" )

#remove forced vfs includes
string(REPLACE "${VFS_INCLUDE}" " " VFS_C_FLAGS ${CMAKE_C_FLAGS} )
string(REPLACE "${VFS_INCLUDE}" " " VFS_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
set( CMAKE_C_FLAGS ${VFS_C_FLAGS})
set (CMAKE_CXX_FLAGS ${VFS_CXX_FLAGS})

# Define needed include files
set ( ZLVFS_INCLUDES
  "${CMAKE_BINARY_DIR}/libmoai/config"
  "${MOAI_ROOT}/src/"
)

# Use all cpp files in directory as source code
file ( GLOB ZLVFS_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/zl-vfs/*.cpp" )
file ( GLOB ZLVFS_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/zl-vfs/*.h" )

include_directories ( ${ZLVFS_INCLUDES} )

add_library ( zlvfs STATIC ${ZLVFS_SOURCES} ${ZLVFS_HEADERS} )

target_link_libraries ( zlvfs zlib tlsf )
set_target_properties( zlvfs PROPERTIES FOLDER Core )
install(TARGETS zlvfs EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES ${ZLVFS_HEADERS} DESTINATION include/zl-vfs)
