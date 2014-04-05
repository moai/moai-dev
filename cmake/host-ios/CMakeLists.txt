cmake_minimum_required ( VERSION 2.8.8 )
project ( moaiios )

message("Building for architecture: ${CMAKE_OSX_ARCHITECTURES}")

add_subdirectory( "${CMAKE_CURRENT_BINARY_DIR}/../bundle_res" "bundle_res")

set ( INCLUDES_MOAIIOS
  "${MOAI_ROOT}/src/"
  "${MOAI_ROOT}/src/config-default/"
  "${MOAI_ROOT}/src/host-glut"
)

set ( IOS_HOST "${MOAI_ROOT}/src/host-ios")

include_directories (
    ${INCLUDES_MOAIIOS}
)

file ( GLOB_RECURSE MOAIIOS_SOURCES
  "${IOS_HOST}/*.cpp"
  "${IOS_HOST}/*.mm"
  "${IOS_HOST}/*.h"
  "${MOAI_ROOT}/src/host-modules/*.cpp"
  "${MOAI_ROOT}/src/host-modules/*.h"
  "${MOAI_ROOT}/src/host-glut/ParticlePresets.cpp"
  "${MOAI_ROOT}/src/host-glut/ParticlePresets.h"
)

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
  host-modules
  moai-sim
  moai-util
  moai-iphone
  moai-audiosampler
  moai-core
  zlcore
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
)

ADD_FRAMEWORKS(moai "${FWORKS}")

set_target_properties(moai PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "${SIGN_IDENTITY}")
set_target_properties(moai PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_LIST_DIR}/moai-Info.plist)

 
