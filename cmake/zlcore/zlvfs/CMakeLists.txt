cmake_minimum_required ( VERSION 2.8.5 )
project ( zlvfs )

# Define source directory
set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/" )

 
# Define needed include files
set ( ZLVFS_INCLUDES
  "${MOAI_ROOT}/src/config-default/" 
  "${MOAI_ROOT}/src/"
 )

# Use all cpp files in directory as source code
file ( GLOB SRC_ZLVFS "${CMAKE_CURRENT_SOURCE_DIR}/zl-vfs/*.cpp" )

include_directories ( ${ZLVFS_INCLUDES} )

add_library ( zlvfs STATIC ${SRC_ZLVFS} )

