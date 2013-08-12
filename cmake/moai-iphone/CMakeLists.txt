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


 CACHE INTERNAL "doc string" )

include_directories ( 
  ${MOAI_IPHONE_INCLUDES} 
)

set ( CMAKE_CURRENT_SOURCE_DIR "../../src/moai-iphone/" )

# Use all cpp files in directory as source code
set ( MOAI_IPHONE_SRC 
	../../src/moai-iphone/AKU-iphone.mm
	../../src/moai-iphone/MOAIAppIOS.mm
	../../src/moai-iphone/MOAIBillingIOS.mm
	../../src/moai-iphone/MOAICrittercismIOS.mm
	../../src/moai-iphone/MOAIDialogIOS.mm
	../../src/moai-iphone/MOAIFacebookIOS.mm
	../../src/moai-iphone/MOAIGameCenterIOS.mm
	../../src/moai-iphone/MOAIKeyboardIOS.mm
	../../src/moai-iphone/MOAIMoviePlayerIOS.mm
	../../src/moai-iphone/MOAINotificationsIOS.mm
	../../src/moai-iphone/MOAISafariIOS.mm
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
)

include_directories ( ${MOAI_IPHONE_INCLUDES} )

add_library ( moai-iphone STATIC ${MOAI_IPHONE_SRC} "${CMAKE_CURRENT_SOURCE_DIR}/Reachability.m" )

 #imported targets


 add_library(libCrittercism STATIC IMPORTED)
 set_property(TARGET libCrittercism PROPERTY
                IMPORTED_LOCATION ../../3rdparty/crittercismiOS-3.3.3/CrittercismSDK-crashonly/libCrittercismCrashOnly_v3_3_3.a)

 add_library(libFacebook STATIC IMPORTED)
 set_property(TARGET libFacebook PROPERTY
                IMPORTED_LOCATION ../../3rdparty/facebookiOS-3.0.6b/libfacebook_ios_sdk.a)


target_link_libraries ( moai-iphone 
	
	libFacebook
	libCrittercism
)