cmake_minimum_required ( VERSION 2.8.8 )
project ( moaiios )

message("Building for architecture: ${CMAKE_OSX_ARCHITECTURES}")




if (HOST_ONLY)
   # define all our required targets
 set ( IOS_HOST "${HOST_PATH}")
  set ( CMAKE_OSX_SYSROOT iphoneos )
 # set ( CMAKE_OSX_ARCHITECTURES "armv7;arm;i386" )
  set ( CMAKE_XCODE_EFFECTIVE_PLATFORMS "-iphoneos;-iphonesimulator" )
  set ( XCODE_ATTRIBUTE_SDKROOT iphoneos )
 message("INCLUDING LIBMOAI")
    include(${LIB_PATH}/cmake/libmoai.cmake)
   set (MOAI_ROOT "${IOS_HOST}/../")
   
    message("adding resources")
   add_subdirectory( "${CMAKE_CURRENT_BINARY_DIR}/bundle_res" "bundle_res")
   add_subdirectory( "../host-modules" "host-modules")
   add_subdirectory( "../moai-iphone/moai-ios-host-modules" "moai-ios-host-modules")
   #add_subdirectory( "../moai-iphone" "moai-iphone")
   message("recursing over sources")
   LINK_DIRECTORIES("${LIB_PATH}/lib")
   file ( GLOB_RECURSE MOAIIOS_SOURCES
     "${IOS_HOST}/host-ios/*.cpp"
     "${IOS_HOST}/host-ios/*.mm"
     "${IOS_HOST}/host-ios/*.h"
   )
   include_directories( ${IOS_HOST} )
   message("End host only section")
else (HOST_ONLY)
add_subdirectory( "${CMAKE_CURRENT_BINARY_DIR}/../bundle_res" "bundle_res")
set ( INCLUDES_MOAIIOS
  "${MOAI_ROOT}/src/"
     "${CMAKE_BINARY_DIR}/libmoai/config"
  "${MOAI_ROOT}/src/host-glut"
)
include_directories (
    ${INCLUDES_MOAIIOS}
)
set ( IOS_HOST "${MOAI_ROOT}/src/host-ios")

 LINK_DIRECTORIES("${LUA_LIB_DIR}")
file ( GLOB_RECURSE MOAIIOS_SOURCES
  "${IOS_HOST}/*.cpp"
  "${IOS_HOST}/*.mm"
  "${IOS_HOST}/*.h"
  "${MOAI_ROOT}/src/host-modules/*.cpp"
  "${MOAI_ROOT}/src/host-modules/*.h"
  "${MOAI_ROOT}/src/host-modules/*.mm"
  "${MOAI_ROOT}/src/host-glut/ParticlePresets.cpp"
  "${MOAI_ROOT}/src/host-glut/ParticlePresets.h"
)

endif (HOST_ONLY)









set ( MOAIIOS_RESOURCES
  ${IOS_HOST}/Icon-72.png
  ${IOS_HOST}/Icon-Small-50.png
  ${IOS_HOST}/Icon-Small.png
  ${IOS_HOST}/Icon-Small@2x.png
  ${IOS_HOST}/Icon.png
  ${IOS_HOST}/Icon@2x.png
)

SET_SOURCE_FILES_PROPERTIES (
  ${MOAIIOS_RESOURCES}
  PROPERTIES
  MACOSX_PACKAGE_LOCATION Resources
)

SET_SOURCE_FILES_PROPERTIES (
  ${PROJECT_RESOURCES}
  PROPERTIES
  MACOSX_PACKAGE_LOCATION moai.app/lua
)

add_executable ( moai MACOSX_BUNDLE ${MOAIIOS_SOURCES} ${MOAIIOS_RESOURCES} ${PROJECT_RESOURCES})


target_link_libraries ( moai
  moai-ios-host-modules
  host-modules
  moai-sim
  moai-util
  moai-iphone
  moai-audiosampler
  moai-core
  zlcore
)

set_target_properties( moai PROPERTIES
   XCODE_ATTRIBUTE_INSTALL_PATH "$(LOCAL_APPS_DIR)"
   XCODE_ATTRIBUTE_SKIP_INSTALL "NO"
   XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "5.1"
   XCODE_ATTRIBUTE_COMBINE_HIDPI_IMAGES "NO"
)

macro(ADD_FRAMEWORKS appname fwnames)
  FOREACH(FW ${fwnames})
    TARGET_LINK_LIBRARIES( ${appname} "-framework ${FW}")
  ENDFOREACH(FW)
endmacro(ADD_FRAMEWORKS)

set( FWORKS
  AdSupport
  AudioToolbox
  CoreAudio
  CoreGraphics
  CoreLocation
  CoreTelephony
  CoreMotion
  CFNetwork
  Foundation
  GameKit
  EventKit
  MediaPlayer
  MobileCoreServices
  MessageUI
  OpenGLES
  QuartzCore
  StoreKit
  Social
  SystemConfiguration
  Twitter
  UIKit
  AVFoundation
  CoreMedia
)
 message("adding frameworks")
ADD_FRAMEWORKS(moai "${FWORKS}")

set_target_properties(moai PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "${SIGN_IDENTITY}")
set_target_properties(moai PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_LIST_DIR}/moai-Info.plist)

 
