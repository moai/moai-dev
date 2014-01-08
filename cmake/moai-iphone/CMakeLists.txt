cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-iphone )

set (LINK_LIBS "")
#frameworks
macro(ADD_EXT_FRAMEWORK fwname altpaths)
   find_library(FRAMEWORK_${fwname}
       NAMES ${fwname}
       PATHS "/Users/david/sdks/moaiforge/3rdparty/Tapjoy.embeddedframework" 
       )

   if( ${FRAMEWORK_${fwname}} STREQUAL FRAMEWORK_${fwname}-NOTFOUND)
       MESSAGE(ERROR ": Framework ${fwname} not found in ${altpaths}")
   else()
       MARK_AS_ADVANCED (${FRAMEWORK_${fwname}})
       set (LINK_LIBS ${LINK_LIBS} ${FRAMEWORK_${fwname}})
       MESSAGE(STATUS "Framework ${fwname} found at ${FRAMEWORK_${fwname}}")
   endif()
endmacro(ADD_EXT_FRAMEWORK)


set ( MOAI_IPHONE_INCLUDES 
  ${THIRD_PARTY_INCLUDES}
  "../../src/"
  "../../src/moai-iphone/"
  "../../src/moai-util/"
  "../../src/config-default/"
  "../../3rdparty/crittercismiOS-4.0.6/CrittercismSDK"
  "../../3rdparty/facebookiOS-3.0.6.b/src"
  "../../3rdparty/playhaven-ios-1.13.1/src"
 CACHE INTERNAL "doc string" )





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

if (NOT DISABLE_TWITTER)
    set ( MOAI_IPHONE_SRC ${MOAI_IPHONE_SRC} 
       ../../src/moai-iphone/MOAITwitterIOS.mm 
   )

endif (NOT DISABLE_TWITTER)



if (NOT DISABLE_TAPJOY)
   set ( MOAI_IPHONE_SRC ${MOAI_IPHONE_SRC} 
       ../../src/moai-iphone/MOAITapjoyIOS.mm 
   )

 set (LINK_LIBS ${LINK_LIBS} "${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/TapjoySDK_iOS_v9.1.0/Libraries/Tapjoy.embeddedframework/Tapjoy.framework")
endif (NOT DISABLE_TAPJOY )


if (NOT DISABLE_VUNGLE)
  set ( MOAI_IPHONE_SRC ${MOAI_IPHONE_SRC}
      ../../src/moai-iphone/MOAIVungleIOS.mm
      ../../src/moai-iphone/AKU-vungle.mm
  )
  set ( MOAI_IPHONE_INCLUDES ${MOAI_IPHONE_INCLUDES} 
     ../../3rdparty/vungle-1.3.3/lib
     ../../3rdparty/vungle-1.3.3/lib/vunglepub
  )
  
   add_library(libVungle STATIC IMPORTED)
   set_property(TARGET libVungle PROPERTY
                IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/vungle-1.3.3/lib/vunglepub/bin/libvunglepub.a)

   set (LINK_LIBS ${LINK_LIBS} libVungle)

endif (NOT DISABLE_VUNGLE)


if (NOT DISABLE_CHARTBOOST)
  set ( MOAI_IPHONE_SRC ${MOAI_IPHONE_SRC}
    ../../src/moai-iphone/MOAIChartBoostIOS.mm
    ../../src/moai-iphone/AKU-chartboost.mm
  )
 
  set ( MOAI_IPHONE_INCLUDES ${MOAI_IPHONE_INCLUDES} 
     ../../3rdparty/chartboostiOS-3.0.7
  )
   add_library(libChartboost STATIC IMPORTED)
   set_property(TARGET libChartboost PROPERTY
                IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/chartboostIOS-3.0.7/libChartboost.a)

   set (LINK_LIBS ${LINK_LIBS} libChartboost)

endif (NOT DISABLE_CHARTBOOST)

if (NOT DISABLE_ADCOLONY)
  set ( MOAI_IPHONE_SRC ${MOAI_IPHONE_SRC}
  	../../src/moai-iphone/MOAIAdColonyIOS.mm
  	../../src/moai-iphone/AKU-adcolony.mm
	)
 
  set ( MOAI_IPHONE_INCLUDES ${MOAI_IPHONE_INCLUDES} 
     ../../3rdparty/adcolonyiOS-1911/Library
  )
   add_library(libAdColony STATIC IMPORTED)
   set_property(TARGET libAdColony PROPERTY
                IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/adcolonyiOS-1911/Library/libAdColony.a)

   set (LINK_LIBS ${LINK_LIBS} libAdColony)

endif (NOT DISABLE_ADCOLONY)




if (NOT DISABLE_PLAYHAVEN)
   set ( MOAI_IPHONE_SRC ${MOAI_IPHONE_SRC} 
       ../../src/moai-iphone/MOAIPlayhavenIOS.mm 
   )
   add_subdirectory(playhaven)
   set (LINK_LIBS ${LINK_LIBS} playhaven-ios)
endif (NOT DISABLE_PLAYHAVEN )


if (NOT DISABLE_CRITTERCISM)
   set ( MOAI_IPHONE_SRC ${MOAI_IPHONE_SRC} 
       ../../src/moai-iphone/MOAICrittercismIOS.mm 
   )

   add_library(libCrittercism STATIC IMPORTED)
   set_property(TARGET libCrittercism PROPERTY
                IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/crittercismiOS-4.0.6/CrittercismSDK/libCrittercism_v4_0_6.a)

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


include_directories ( 
  ${MOAI_IPHONE_INCLUDES} 
)


target_link_libraries ( moai-iphone 
	${LINK_LIBS}
)
