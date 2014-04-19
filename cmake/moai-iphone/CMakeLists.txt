cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-iphone )

set (LINK_LIBS "")


set ( MOAI_IPHONE_INCLUDES 
  "${MOAI_ROOT}/src/"
  "${MOAI_ROOT}/src/moai-iphone/"
  "${MOAI_ROOT}/src/config-default/"
  "${MOAI_ROOT}/3rdparty/crittercismiOS-4.0.6/CrittercismSDK"
  "${MOAI_ROOT}/3rdparty/facebookiOS-3.0.6.b/src"
  "${MOAI_ROOT}/3rdparty/playhaven-ios-1.13.1/src"
  "${MOAI_ROOT}/3rdparty/vungle-1.3.3/lib"
  "${MOAI_ROOT}/3rdparty/vungle-1.3.3/lib/vunglepub"
  "${MOAI_ROOT}/3rdparty/chartboostiOS-3.0.7"
  "${MOAI_ROOT}/3rdparty/adcolonyiOS-1911/Library"
)

# Use all mm files in directory as source code
file ( GLOB MOAI_IPHONE_SRC 
  "${MOAI_ROOT}/src/moai-iphone/*.mm"
  "${MOAI_ROOT}/src/moai-iphone/*.m"
  "${MOAI_ROOT}/src/moai-iphone/*.h"
  "${MOAI_ROOT}/3rdparty/contrib/ISO8601DateFormatter.m"
  "${MOAI_ROOT}/3rdparty/openUDID/MOAIOpenUDID.m"
)

# Extensions

if (NOT DISABLE_TAPJOY )
  set (LINK_LIBS ${LINK_LIBS} "${MOAI_ROOT}/3rdparty/TapjoySDK_iOS_v9.1.0/Libraries/Tapjoy.embeddedframework/Tapjoy.framework")
endif (NOT DISABLE_TAPJOY )
install(directory "${MOAI_ROOT}/3rdparty/TapjoySDK_iOS_v9.1.0/Libraries/Tapjoy.embeddedframework/Tapjoy.framework" DESTINATION lib)


set(VUNGLE_LIB ${MOAI_ROOT}/3rdparty/vungle-1.3.3/lib/vunglepub/bin/libvunglepub.a)
add_library(libVungle STATIC IMPORTED)
set_property(TARGET libVungle PROPERTY IMPORTED_LOCATION ${VUNGLE_LIB})

install(FILES ${VUNGLE_LIB} DESTINATION lib)

if (NOT DISABLE_VUNGLE)
  set (LINK_LIBS ${LINK_LIBS} libVungle)
endif (NOT DISABLE_VUNGLE)

set(CHARTBOOST_LIB ${MOAI_ROOT}/3rdparty/chartboostIOS-3.0.7/libChartboost.a)
add_library(libChartboost STATIC IMPORTED)
set_property(TARGET libChartboost PROPERTY IMPORTED_LOCATION ${CHARTBOOST_LIB})

install(FILES ${CHARTBOOST_LIB} DESTINATION lib)

if (NOT DISABLE_CHARTBOOST)
  set (LINK_LIBS ${LINK_LIBS} libChartboost)
endif (NOT DISABLE_CHARTBOOST)

set(ADCOLONY_LIB ${MOAI_ROOT}/3rdparty/adcolonyiOS-1911/Library/libAdColony.a)
add_library(libAdColony STATIC IMPORTED)
set_property(TARGET libAdColony PROPERTY IMPORTED_LOCATION ${ADCOLONY_LIB})

install(FILES ${ADCOLONY_LIB} DESTINATION lib)

if (NOT DISABLE_ADCOLONY)
  set (LINK_LIBS ${LINK_LIBS} libAdColony)
endif (NOT DISABLE_ADCOLONY)



add_subdirectory(playhaven)
if (NOT DISABLE_PLAYHAVEN)
  set (LINK_LIBS ${LINK_LIBS} playhaven-ios)
endif (NOT DISABLE_PLAYHAVEN )


set(CRITTERCISM_LIB ${MOAI_ROOT}/3rdparty/crittercismiOS-4.0.6/CrittercismSDK/libCrittercism_v4_0_6.a)
add_library(libCrittercism STATIC IMPORTED)
set_property(TARGET libCrittercism PROPERTY IMPORTED_LOCATION ${CRITTERCISM_LIB})

install(FILES ${CRITTERCISM_LIB} DESTINATION lib)

if (NOT DISABLE_CRITTERCISM)
  set (LINK_LIBS ${LINK_LIBS} libCrittercism)
endif (NOT DISABLE_CRITTERCISM)


set(FACEBOOK_LIB ${MOAI_ROOT}/3rdparty/facebookiOS-3.0.6.b/libfacebook_ios_sdk.a)
add_library(libFacebook STATIC IMPORTED)
set_property(TARGET libFacebook PROPERTY IMPORTED_LOCATION ${FACEBOOK_LIB})

install(FILES ${FACEBOOK_LIB} DESTINATION lib)
 
if (NOT DISABLE_FACEBOOK)
  if (NOT MOAI_SQLITE3)
    message(FATAL_ERROR "Facebook plugin requires MOAI_SQLITE3")
  endif (NOT MOAI_SQLITE3)

  set (LINK_LIBS ${LINK_LIBS} libFacebook sqlite3)
endif (NOT DISABLE_FACEBOOK)


if (NOT DISABLE_MOBILEAPPTRACKER)
  set (LINK_LIBS ${LINK_LIBS} "${MOAI_ROOT}/3rdparty/MobileAppTracking-ios/MobileAppTracker.framework")
endif (NOT DISABLE_MOBILEAPPTRACKER)

install(directory "${MOAI_ROOT}/3rdparty/MobileAppTracking-ios/MobileAppTracker.framework" DESTINATION lib)

include_directories ( ${MOAI_IPHONE_INCLUDES} )
add_library ( moai-iphone STATIC ${MOAI_IPHONE_SRC}  )

target_link_libraries ( moai-iphone 
  ${LINK_LIBS}
  moai-sim moai-util moai-core 
)

target_include_directories( moai-iphone PUBLIC 
  "$<BUILD_INTERFACE:${MOAI_IPHONE_INCLUDES}>"
  "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
)

set_target_properties( moai-iphone PROPERTIES FOLDER Modules )
install(TARGETS moai-iphone EXPORT libmoai ARCHIVE DESTINATION lib)
file ( GLOB IPHONE_AKU "${MOAI_ROOT}/src/moai-iphone/AKU-*.h")
install(FILES ${IPHONE_AKU} DESTINATION "include/moai-iphone")

