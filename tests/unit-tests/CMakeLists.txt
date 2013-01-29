cmake_minimum_required ( VERSION 2.8.5 )
project ( unit-tests )

# Add the include directory for Google Test and 3rd party stuff
include_directories ( "${ROOT_DIR}./gtest/include/" )
include_directories ( "${ROOT_DIR}../../src/" )
include_directories ( "${ROOT_DIR}../../src/config-default" )
include_directories ( "${ROOT_DIR}../../3rdparty/" )
include_directories ( "${ROOT_DIR}../../3rdparty/lua-5.1.3/src" )
include_directories ( "${ROOT_DIR}../../3rdparty/freetype-2.4.4/include" )
include_directories ( "${ROOT_DIR}../../3rdparty/box2d-2.2.1/" )


# Build google test as well
add_subdirectory ( gtest )
#add_subdirectory ( ../../cmake  "${CMAKE_CURRENT_BINARY_DIR}/builds" ) 

# grab all the tests
file(GLOB_RECURSE tests "src/*" )

# Create the main executable
add_executable ( unit-tests moai-unit-tests.cc ${tests} )

# Configure build options
add_definitions(-fprofile-arcs -ftest-coverage)


# requirements
target_link_libraries ( unit-tests gtest )
target_link_libraries ( unit-tests gcov )
#target_link_libraries ( unit-tests moaicore )
#target_link_libraries ( unit-tests aku )
