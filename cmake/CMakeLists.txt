#
# This is the CMake build script for Moai SDK library
# Please, don't modify the flags inside this script.
# To create a custom version of Moai SDK use a bash script
# like the ones in bin/ ( bin/build-linux_x64 for example )
#

cmake_minimum_required ( VERSION 2.8.5 )
project ( moai )


# Setup the "MOAI_ROOT" variable
get_filename_component(MOAI_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")


if (BUILD_ANDROID) 
  include(cmake-modules/platform-android.cmake)
endif (BUILD_ANDROID)

if (BUILD_IOS)
  include(cmake-modules/platform-ios.cmake)
endif (BUILD_IOS)

if (BUILD_LINUX)
  include(cmake-modules/platform-linux.cmake)
endif (BUILD_LINUX)

if (BUILD_OSX)
  include(cmake-modules/platform-osx.cmake)
endif (BUILD_OSX)

if (BUILD_WINDOWS)
  include(cmake-modules/platform-windows.cmake)
endif (BUILD_WINDOWS)

if (BUILD_HTML)
  include(cmake-modules/platform-html.cmake) 
endif (BUILD_HTML)

#common setup
include(cmake-modules/platform-common.cmake)
include(cmake-modules/module-setup.cmake)

add_subdirectory(libmoai)

#install target so we can use a prebuilt one next time
install(EXPORT libmoai DESTINATION cmake )
