cmake_minimum_required ( VERSION 2.8.8 )
project ( moaiios )



message("Building for architecture: ${CMAKE_OSX_ARCHITECTURES}")


   FOREACH(DISABLED ${DISABLED_EXT} )
     set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDISABLE_${DISABLED}" CACHE INTERNAL "doc string" )
     set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDISABLE_${DISABLED}" CACHE INTERNAL "doc string" )
     set ( DISABLE_${DISABLED} TRUE )
   ENDFOREACH()

add_subdirectory( "${CMAKE_CURRENT_BINARY_DIR}/../bundle_res" "bundle_res")

set ( INCLUDES_MOAIIOS
  "${MOAI_ROOT}/src/"
  "${MOAI_ROOT}/src/config-default/"
  "${MOAI_ROOT}/3rdparty/lua-5.1.3/src"
  "${MOAI_ROOT}/3rdparty/expat-2.0.1/lib"
  "${MOAI_ROOT}/3rdparty/tinyxml"
  "${MOAI_ROOT}/3rdparty"
  "${MOAI_ROOT}/3rdparty/freetype-2.4.4/include"
  "${MOAI_ROOT}/src/host-glut"
    CACHE INTERNAL "doc string" )

set ( IOS_HOST "${MOAI_ROOT}/xcode/ios")

include_directories (
    ${INCLUDES_MOAIIOS}
)



set ( MOAIIOS_SOURCES
     ${IOS_HOST}/main.mm
     ${IOS_HOST}/Classes/OpenGLView.mm
     ${IOS_HOST}/Classes/MoaiView.mm
     ${IOS_HOST}/Classes/MoaiVC.mm
     ${IOS_HOST}/Classes/MoaiAppDelegate.mm
     ${IOS_HOST}/Classes/LocationObserver.mm
     ${MOAI_ROOT}/src/host-glut/ParticlePresets.cpp
     ${MOAI_ROOT}/src/host-modules/aku_modules.cpp
)

set ( MOAIIOS_RESOURCES
     ${IOS_HOST}/Icon-72.png
     ${IOS_HOST}/Icon-Small-50.png
     ${IOS_HOST}/Icon-Small.png
     ${IOS_HOST}/Icon-Small@2x.png
     ${IOS_HOST}/Icon.png
     ${IOS_HOST}/Icon@2x.png
)


  SET_SOURCE_FILES_PROPERTIES(
    ${MOAIIOS_RESOURCES}
    PROPERTIES
    MACOSX_PACKAGE_LOCATION Resources
    )


  SET_SOURCE_FILES_PROPERTIES(
    ${PROJECT_RESOURCES}
    PROPERTIES
    MACOSX_PACKAGE_LOCATION moai.app/lua
  )



     
add_executable ( moai MACOSX_BUNDLE ${MOAIIOS_SOURCES} ${MOAIIOS_RESOURCES} ${PROJECT_RESOURCES})


target_link_libraries ( moai
   moai-sim
   moai-util
   moai-iphone
   moai-audiosampler
   moai-core
   zlcore
   third-party
)

if (MOAI_LUAEXT )
    target_link_libraries( moai  moai-luaext )
endif( MOAI_LUAEXT )

if ( MOAI_HTTP_CLIENT)
    target_link_libraries( moai moai-http-client)
endif (MOAI_HTTP_CLIENT)

if ( MOAI_UNTZ )
   target_link_libraries( moai moai-untz )
endif (MOAI_UNTZ)

if ( MOAI_CHIPMUNK )
  target_link_libraries ( moai moai-chipmunk )
endif ( MOAI_CHIPMUNK )

if ( MOAI_BOX2D )
  target_link_libraries ( moai moai-box2d )
endif ( MOAI_BOX2D )

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
                  UIKit)

ADD_FRAMEWORKS(moai "${FWORKS}")






set_target_properties(moai PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "${SIGN_IDENTITY}")
set_target_properties(moai PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_LIST_DIR}/moai-Info.plist)

 
