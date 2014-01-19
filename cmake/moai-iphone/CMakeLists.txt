cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-iphone )

set (LINK_LIBS "")



set ( MOAI_IPHONE_INCLUDES 
  "${MOAI_ROOT}/src/"
  "${MOAI_ROOT}/src/moai-iphone/"
  "${MOAI_ROOT}/src/moai-util/"
  "${MOAI_ROOT}/src/config-default/"
  "${MOAI_ROOT}/3rdparty/crittercismiOS-4.0.6/CrittercismSDK"
  "${MOAI_ROOT}/3rdparty/facebookiOS-3.0.6.b/src"
  "${MOAI_ROOT}/3rdparty/playhaven-ios-1.13.1/src"
   ${MOAI_ROOT}/3rdparty/vungle-1.3.3/lib
   ${MOAI_ROOT}/3rdparty/vungle-1.3.3/lib/vunglepub
   ${MOAI_ROOT}/3rdparty/chartboostiOS-3.0.7
    ${MOAI_ROOT}/3rdparty/adcolonyiOS-1911/Library
  
  )





# Use all cpp files in directory as source code
set ( MOAI_IPHONE_SRC 
	${MOAI_ROOT}/src/moai-iphone/AKU-iphone.mm
	${MOAI_ROOT}/src/moai-iphone/MOAIAppIOS.mm
	${MOAI_ROOT}/src/moai-iphone/MOAIBillingIOS.mm
	
	${MOAI_ROOT}/src/moai-iphone/MOAIDialogIOS.mm

	${MOAI_ROOT}/src/moai-iphone/MOAIGameCenterIOS.mm
	${MOAI_ROOT}/src/moai-iphone/MOAIKeyboardIOS.mm
	${MOAI_ROOT}/src/moai-iphone/MOAIMoviePlayerIOS.mm
	${MOAI_ROOT}/src/moai-iphone/MOAINotificationsIOS.mm
	${MOAI_ROOT}/src/moai-iphone/MOAIBrowserIOS.mm
	${MOAI_ROOT}/src/moai-iphone/MOAIStoreKitListener.mm
	${MOAI_ROOT}/src/moai-iphone/MOAITakeCameraListener.mm
	${MOAI_ROOT}/src/moai-iphone/MOAIWebViewDelegate.mm
	${MOAI_ROOT}/src/moai-iphone/MOAIWebViewIOS.mm      
	${MOAI_ROOT}/src/moai-iphone/NSArray+MOAILib.mm
	${MOAI_ROOT}/src/moai-iphone/NSData+MOAILib.mm
	${MOAI_ROOT}/src/moai-iphone/NSDate+MOAILib.mm
	${MOAI_ROOT}/src/moai-iphone/NSDictionary+MOAILib.mm
	${MOAI_ROOT}/src/moai-iphone/NSError+MOAILib.mm
	${MOAI_ROOT}/src/moai-iphone/NSNumber+MOAILib.mm
	${MOAI_ROOT}/src/moai-iphone/NSObject+MOAILib.mm
	${MOAI_ROOT}/src/moai-iphone/NSString+MOAILib.mm
	${MOAI_ROOT}/src/moai-iphone/Reachability.m
	${MOAI_ROOT}/src/moai-iphone/ReachabilityListener.mm
	${MOAI_ROOT}/3rdparty/contrib/ISO8601DateFormatter.m
	${MOAI_ROOT}/3rdparty/openUDID/MOAIOpenUDID.m

  ${MOAI_ROOT}/src/moai-iphone/MOAITwitterIOS.mm
  ${MOAI_ROOT}/src/moai-iphone/MOAITapjoyIOS.mm 

  ${MOAI_ROOT}/src/moai-iphone/MOAIVungleIOS.mm
  ${MOAI_ROOT}/src/moai-iphone/AKU-vungle.mm

     ${MOAI_ROOT}/src/moai-iphone/MOAIChartBoostIOS.mm
    ${MOAI_ROOT}/src/moai-iphone/AKU-chartboost.mm

        ${MOAI_ROOT}/src/moai-iphone/MOAIAdColonyIOS.mm
    ${MOAI_ROOT}/src/moai-iphone/AKU-adcolony.mm

    ${MOAI_ROOT}/src/moai-iphone/MOAIPlayhavenIOS.mm 

     ${MOAI_ROOT}/src/moai-iphone/MOAICrittercismIOS.mm 

      ${MOAI_ROOT}/src/moai-iphone/MOAIFacebookIOS.mm

       ${MOAI_ROOT}/src/moai-iphone/MOAIMobileAppTrackerIOS.mm 
)



# Extensions

if ( DISABLE_TWITTER)
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDISABLE_TWITTER=1"  PARENT_SCOPE )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDISABLE_TWITTER=1"  PARENT_SCOPE )
endif (DISABLE_TWITTER)



if (NOT DISABLE_TAPJOY)
    set (LINK_LIBS ${LINK_LIBS} "${MOAI_ROOT}/3rdparty/TapjoySDK_iOS_v9.1.0/Libraries/Tapjoy.embeddedframework/Tapjoy.framework")
else ()
    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDISABLE_TAPJOY=1"  PARENT_SCOPE )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDISABLE_TAPJOY=1"  PARENT_SCOPE )
endif (NOT DISABLE_TAPJOY )


add_library(libVungle STATIC IMPORTED)
set_property(TARGET libVungle PROPERTY
              IMPORTED_LOCATION ${MOAI_ROOT}/3rdparty/vungle-1.3.3/lib/vunglepub/bin/libvunglepub.a)


if (NOT DISABLE_VUNGLE)
   set (LINK_LIBS ${LINK_LIBS} libVungle)
else ()
    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDISABLE_VUNGLE=1"  PARENT_SCOPE )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDISABLE_VUNGLE=1"  PARENT_SCOPE )
endif (NOT DISABLE_VUNGLE)



add_library(libChartboost STATIC IMPORTED)
set_property(TARGET libChartboost PROPERTY
                IMPORTED_LOCATION ${MOAI_ROOT}/3rdparty/chartboostIOS-3.0.7/libChartboost.a)

if (NOT DISABLE_CHARTBOOST)
   set (LINK_LIBS ${LINK_LIBS} libChartboost)
else ()
    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDISABLE_CHARTBOOST=1"  PARENT_SCOPE )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDISABLE_CHARTBOOST=1"  PARENT_SCOPE )
endif (NOT DISABLE_CHARTBOOST)

add_library(libAdColony STATIC IMPORTED)
set_property(TARGET libAdColony PROPERTY
                IMPORTED_LOCATION ${MOAI_ROOT}/3rdparty/adcolonyiOS-1911/Library/libAdColony.a)

if (NOT DISABLE_ADCOLONY)
  set (LINK_LIBS ${LINK_LIBS} libAdColony)
else ()
    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDISABLE_ADCOLONY=1"  PARENT_SCOPE )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDISABLE_ADCOLONY=1"  PARENT_SCOPE )
endif (NOT DISABLE_ADCOLONY)


add_subdirectory(playhaven)
if (NOT DISABLE_PLAYHAVEN)
   set (LINK_LIBS ${LINK_LIBS} playhaven-ios)
else()
   set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDISABLE_PLAYHAVEN=1"  PARENT_SCOPE )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDISABLE_PLAYHAVEN=1"  PARENT_SCOPE )
endif (NOT DISABLE_PLAYHAVEN )


add_library(libCrittercism STATIC IMPORTED)
set_property(TARGET libCrittercism PROPERTY
              IMPORTED_LOCATION ${MOAI_ROOT}/3rdparty/crittercismiOS-4.0.6/CrittercismSDK/libCrittercism_v4_0_6.a)

if (NOT DISABLE_CRITTERCISM)
   set (LINK_LIBS ${LINK_LIBS} libCrittercism)
else()
    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDISABLE_CRITTERCISM=1"  PARENT_SCOPE )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDISABLE_CRITTERCISM=1"  PARENT_SCOPE )
endif (NOT DISABLE_CRITTERCISM )


add_library(libFacebook STATIC IMPORTED)
set_property(TARGET libFacebook PROPERTY
                IMPORTED_LOCATION ${MOAI_ROOT}/3rdparty/facebookiOS-3.0.6.b/libfacebook_ios_sdk.a)
 
if (NOT DISABLE_FACEBOOK)
 set (LINK_LIBS ${LINK_LIBS} libFacebook)
 else()
    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDISABLE_FACEBOOK=1"  PARENT_SCOPE )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDISABLE_FACEBOOK=1"  PARENT_SCOPE )
endif (NOT DISABLE_FACEBOOK)


if (NOT DISABLE_MOBILEAPPTRACKER)
  set (LINK_LIBS ${LINK_LIBS} "${MOAI_ROOT}/3rdparty/MobileAppTracking-ios/MobileAppTracker.framework")
  else()
    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDISABLE_MOBILEAPPTRACKER=1"  PARENT_SCOPE )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDISABLE_MOBILEAPPTRACKER=1"  PARENT_SCOPE )
endif (NOT DISABLE_MOBILEAPPTRACKER)

include_directories ( ${MOAI_IPHONE_INCLUDES} )
add_library ( moai-iphone STATIC ${MOAI_IPHONE_SRC}  )

target_link_libraries ( moai-iphone 
	${LINK_LIBS}
  third-party
)
