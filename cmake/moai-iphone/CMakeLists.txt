cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-iphone )

set ( MOAI_IPHONE_INCLUDES 
  ${THIRD_PARTY_INCLUDES}
  "../../src/"
  "../../src/moai-iphone/"
  "../../src/moai-util/"
  "../../src/config-default/"
  "../../3rdparty/crittercismiOS-3.3.3/CrittercismSDK-crashonly"
  "../../3rdparty/facebookiOS-3.0.6.b/src"
  "../../3rdparty/playhaven-ios-1.13.1/src"


 CACHE INTERNAL "doc string" )

include_directories ( 
  ${MOAI_IPHONE_INCLUDES} 
)



# Use all cpp files in directory as source code
set ( MOAI_IPHONE_SRC 
	../../src/moai-iphone/AKU-iphone.mm
	../../src/moai-iphone/MOAIAppIOS.mm
	../../src/moai-iphone/MOAIBillingIOS.mm
	
	../../src/moai-iphone/MOAIDialogIOS.mm

	../../src/moai-iphone/MOAIGameCenterIOS.mm
	../../src/moai-iphone/MOAIKeyboardIOS.mm
	../../src/moai-iphone/MOAIMoviePlayerIOS.mm
	../../src/moai-iphone/MOAINotificationsIOS.mm
	../../src/moai-iphone/MOAIBrowserIOS.mm
	../../src/moai-iphone/MOAIStoreKitListener.mm
	../../src/moai-iphone/MOAITakeCameraListener.mm
	../../src/moai-iphone/MOAITwitterIOS.mm
	../../src/moai-iphone/MOAIWebViewDelegate.mm
	../../src/moai-iphone/MOAIWebViewIOS.mm      
	../../src/moai-iphone/NSArray+MOAILib.mm
	../../src/moai-iphone/NSData+MOAILib.mm
	../../src/moai-iphone/NSDate+MOAILib.mm
	../../src/moai-iphone/NSDictionary+MOAILib.mm
	../../src/moai-iphone/NSError+MOAILib.mm
	../../src/moai-iphone/NSNumber+MOAILib.mm
	../../src/moai-iphone/NSObject+MOAILib.mm
	../../src/moai-iphone/NSString+MOAILib.mm
	../../src/moai-iphone/Reachability.m
	../../src/moai-iphone/ReachabilityListener.mm
	../../3rdparty/contrib/ISO8601DateFormatter.m
	../../3rdparty/openUDID/MOAIOpenUDID.m
)



# Extensions

#     adcolony: no
#     chartboost: yes
#     tapjoy: no
#     twitter: yes
#     vungle: no


set (LINK_LIBS "")


if (NOT DISABLE_PLAYHAVEN)
   set ( MOAI_IPHONE_SRC ${MOAI_IPHONE_SRC} 
       ../../src/moai-iphone/MOAIPlayhavenIOS.mm 
   )

   include_directories (../../3rdparty/playhaven-ios-1.13.1/src  ../../3rdparty/playhaven-ios-1.13.1/JSON ../../3rdparty/playhaven-ios-1.13.1/Cache)
   file(GLOB PLAYHAVEN_SOURCE ../../3rdparty/playhaven-ios-1.13.1/src/*.m  ../../3rdparty/playhaven-ios-1.13.1/JSON/*.m ../../3rdparty/playhaven-ios-1.13.1/Cache/*.m)
   add_library(playhaven STATIC ${PLAYHAVEN_SOURCE})
   
   set (LINK_LIBS ${LINK_LIBS} playhaven)

endif (NOT DISABLE_PLAYHAVEN )


if (NOT DISABLE_CRITTERCISM)
   set ( MOAI_IPHONE_SRC ${MOAI_IPHONE_SRC} 
       ../../src/moai-iphone/MOAICrittercismIOS.mm 
   )

   add_library(libCrittercism STATIC IMPORTED)
   set_property(TARGET libCrittercism PROPERTY
                IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/crittercismiOS-3.3.3/CrittercismSDK-crashonly/libCrittercismCrashOnly_v3_3_3.a)

   set (LINK_LIBS ${LINK_LIBS} libCrittercism)

endif (NOT DISABLE_CRITTERCISM )


if (NOT DISABLE_FACEBOOK)
 set ( MOAI_IPHONE_SRC ${MOAI_IPHONE_SRC} 
    	../../src/moai-iphone/MOAIFacebookIOS.mm
 )

 add_library(libFacebook STATIC IMPORTED)
 set_property(TARGET libFacebook PROPERTY
                IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/facebookiOS-3.0.6.b/libfacebook_ios_sdk.a)
  
 set (LINK_LIBS ${LINK_LIBS} libFacebook)
endif (NOT DISABLE_FACEBOOK)


include_directories ( ${MOAI_IPHONE_INCLUDES} )
add_library ( moai-iphone STATIC ${MOAI_IPHONE_SRC}  )





target_link_libraries ( moai-iphone 
	${LINK_LIBS}
)
