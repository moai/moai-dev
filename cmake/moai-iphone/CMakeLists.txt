cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-iphone )

set (LINK_LIBS "")



set ( MOAI_IPHONE_INCLUDES 
  ${THIRD_PARTY_INCLUDES}
  "${MOAI_ROOT}/src/"
  "${MOAI_ROOT}/src/moai-iphone/"
  "${MOAI_ROOT}/src/moai-util/"
  "${MOAI_ROOT}/src/config-default/"
  "${MOAI_ROOT}/3rdparty/crittercismiOS-4.0.6/CrittercismSDK"
  "${MOAI_ROOT}/3rdparty/facebookiOS-3.0.6.b/src"
  "${MOAI_ROOT}/3rdparty/playhaven-ios-1.13.1/src"
  
 CACHE INTERNAL "doc string" )





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
)



# Extensions

if (NOT DISABLE_TWITTER)
    set ( MOAI_IPHONE_SRC ${MOAI_IPHONE_SRC} 
       ${MOAI_ROOT}/src/moai-iphone/MOAITwitterIOS.mm 
   )

endif (NOT DISABLE_TWITTER)



if (NOT DISABLE_TAPJOY)
   set ( MOAI_IPHONE_SRC ${MOAI_IPHONE_SRC} 
       ${MOAI_ROOT}/src/moai-iphone/MOAITapjoyIOS.mm 
   )

 set (LINK_LIBS ${LINK_LIBS} "${MOAI_ROOT}/3rdparty/TapjoySDK_iOS_v9.1.0/Libraries/Tapjoy.embeddedframework/Tapjoy.framework")
endif (NOT DISABLE_TAPJOY )


if (NOT DISABLE_VUNGLE)
  set ( MOAI_IPHONE_SRC ${MOAI_IPHONE_SRC}
      ${MOAI_ROOT}/src/moai-iphone/MOAIVungleIOS.mm
      ${MOAI_ROOT}/src/moai-iphone/AKU-vungle.mm
  )
  set ( MOAI_IPHONE_INCLUDES ${MOAI_IPHONE_INCLUDES} 
     ${MOAI_ROOT}/3rdparty/vungle-1.3.3/lib
     ${MOAI_ROOT}/3rdparty/vungle-1.3.3/lib/vunglepub
  )
  
   add_library(libVungle STATIC IMPORTED)
   set_property(TARGET libVungle PROPERTY
                IMPORTED_LOCATION ${MOAI_ROOT}/3rdparty/vungle-1.3.3/lib/vunglepub/bin/libvunglepub.a)

   set (LINK_LIBS ${LINK_LIBS} libVungle)

endif (NOT DISABLE_VUNGLE)


if (NOT DISABLE_CHARTBOOST)
  set ( MOAI_IPHONE_SRC ${MOAI_IPHONE_SRC}
    ${MOAI_ROOT}/src/moai-iphone/MOAIChartBoostIOS.mm
    ${MOAI_ROOT}/src/moai-iphone/AKU-chartboost.mm
  )
 
  set ( MOAI_IPHONE_INCLUDES ${MOAI_IPHONE_INCLUDES} 
     ${MOAI_ROOT}/3rdparty/chartboostiOS-3.0.7
  )
   add_library(libChartboost STATIC IMPORTED)
   set_property(TARGET libChartboost PROPERTY
                IMPORTED_LOCATION ${MOAI_ROOT}/3rdparty/chartboostIOS-3.0.7/libChartboost.a)

   set (LINK_LIBS ${LINK_LIBS} libChartboost)

endif (NOT DISABLE_CHARTBOOST)

if (NOT DISABLE_ADCOLONY)
  set ( MOAI_IPHONE_SRC ${MOAI_IPHONE_SRC}
  	${MOAI_ROOT}/src/moai-iphone/MOAIAdColonyIOS.mm
  	${MOAI_ROOT}/src/moai-iphone/AKU-adcolony.mm
	)
 
  set ( MOAI_IPHONE_INCLUDES ${MOAI_IPHONE_INCLUDES} 
     ${MOAI_ROOT}/3rdparty/adcolonyiOS-1911/Library
  )
   add_library(libAdColony STATIC IMPORTED)
   set_property(TARGET libAdColony PROPERTY
                IMPORTED_LOCATION ${MOAI_ROOT}/3rdparty/adcolonyiOS-1911/Library/libAdColony.a)

   set (LINK_LIBS ${LINK_LIBS} libAdColony)

endif (NOT DISABLE_ADCOLONY)




if (NOT DISABLE_PLAYHAVEN)
   set ( MOAI_IPHONE_SRC ${MOAI_IPHONE_SRC} 
       ${MOAI_ROOT}/src/moai-iphone/MOAIPlayhavenIOS.mm 
   )
   add_subdirectory(playhaven)
   set (LINK_LIBS ${LINK_LIBS} playhaven-ios)
endif (NOT DISABLE_PLAYHAVEN )


if (NOT DISABLE_CRITTERCISM)
   set ( MOAI_IPHONE_SRC ${MOAI_IPHONE_SRC} 
       ${MOAI_ROOT}/src/moai-iphone/MOAICrittercismIOS.mm 
   )

   add_library(libCrittercism STATIC IMPORTED)
   set_property(TARGET libCrittercism PROPERTY
                IMPORTED_LOCATION ${MOAI_ROOT}/3rdparty/crittercismiOS-4.0.6/CrittercismSDK/libCrittercism_v4_0_6.a)

   set (LINK_LIBS ${LINK_LIBS} libCrittercism)

endif (NOT DISABLE_CRITTERCISM )


if (NOT DISABLE_FACEBOOK)
 set ( MOAI_IPHONE_SRC ${MOAI_IPHONE_SRC} 
    	${MOAI_ROOT}/src/moai-iphone/MOAIFacebookIOS.mm
 )

 add_library(libFacebook STATIC IMPORTED)
 set_property(TARGET libFacebook PROPERTY
                IMPORTED_LOCATION ${MOAI_ROOT}/3rdparty/facebookiOS-3.0.6.b/libfacebook_ios_sdk.a)
  
 set (LINK_LIBS ${LINK_LIBS} libFacebook)
endif (NOT DISABLE_FACEBOOK)


if (NOT DISABLE_MOBILEAPPTRACKER)
   set ( MOAI_IPHONE_SRC ${MOAI_IPHONE_SRC} 
       ${MOAI_ROOT}/src/moai-iphone/MOAIMobileAppTrackerIOS.mm 
   )

 set (LINK_LIBS ${LINK_LIBS} "${MOAI_ROOT}/3rdparty/MobileAppTracking-ios/MobileAppTracker.framework")

endif (NOT DISABLE_MOBILEAPPTRACKER)

include_directories ( ${MOAI_IPHONE_INCLUDES} )
add_library ( moai-iphone STATIC ${MOAI_IPHONE_SRC}  )


include_directories ( 
  ${MOAI_IPHONE_INCLUDES} 
)


target_link_libraries ( moai-iphone 
	${LINK_LIBS}
)
