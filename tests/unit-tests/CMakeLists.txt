cmake_minimum_required ( VERSION 2.8.5 )
project ( unit-tests )

# Add the include directory for Google Test
include_directories ( "${ROOT_DIR}./gtest/include/" )

# Build google test as well
add_subdirectory ( gtest ) 

# grab all the tests
file(GLOB_RECURSE files "src/*" )

# Create the main executable
add_executable ( moai-unit-tests moai-unit-tests.cc ${files} )

target_link_libraries ( moai-unit-tests gtest )