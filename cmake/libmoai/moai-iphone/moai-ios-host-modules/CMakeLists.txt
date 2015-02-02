cmake_minimum_required ( VERSION 2.8.11 )
project ( moai-ios-host-modules)

set ( HOST_MODULES_INCLUDES 
  "${MOAI_ROOT}/src"
  "${CMAKE_BINARY_DIR}/libmoai/config"
)

# Use all cpp files in directory as source code
file ( GLOB HOST_MODULES_SRC
  "${MOAI_ROOT}/src/host-modules/aku_modules_ios.*"
  "${MOAI_ROOT}/src/host-modules/aku_modules_ios_config.h"
)

include_directories ( ${HOST_MODULES_INCLUDES} )

add_library ( moai-ios-host-modules STATIC ${HOST_MODULES_SRC} )

set_target_properties( moai-ios-host-modules PROPERTIES FOLDER Modules )
 
target_link_libraries ( moai-ios-host-modules host-modules)

# adcolony doesnt have armv7s, so leave it as opt in
#if ( NOT DISABLE_ADCOLONY )
#  target_link_libraries ( moai-ios-host-modules moai-ios-adcolony )
#endif ( NOT DISABLE_ADCOLONY )


if ( NOT DISABLE_BILLING )
  target_link_libraries ( moai-ios-host-modules moai-ios-billing )
endif ( NOT DISABLE_BILLING )


if ( NOT DISABLE_CHARTBOOST )
  target_link_libraries ( moai-ios-host-modules moai-ios-chartboost )
endif ( NOT DISABLE_CHARTBOOST )


if ( NOT DISABLE_CRITTERCISM )
  target_link_libraries ( moai-ios-host-modules moai-ios-crittercism )
endif ( NOT DISABLE_CRITTERCISM )


if ( NOT DISABLE_FACEBOOK )
  target_link_libraries ( moai-ios-host-modules moai-ios-facebook )
endif ( NOT DISABLE_FACEBOOK )


if ( NOT DISABLE_GAMECENTER )
	target_link_libraries ( moai-ios-host-modules moai-ios-gamecenter )
endif ( NOT DISABLE_GAMECENTER )


if ( NOT DISABLE_MOBILEAPPTRACKER )
  target_link_libraries ( moai-ios-host-modules moai-ios-mobile-app-tracker )
endif ( NOT DISABLE_MOBILEAPPTRACKER )


if ( NOT DISABLE_PLAYHAVEN )
  target_link_libraries ( moai-ios-host-modules moai-ios-playhaven )
endif ( NOT DISABLE_PLAYHAVEN )


if ( NOT DISABLE_TAPJOY )
  target_link_libraries ( moai-ios-host-modules moai-ios-tapjoy )
endif ( NOT DISABLE_TAPJOY)


if ( NOT DISABLE_TWITTER )
  target_link_libraries ( moai-ios-host-modules moai-ios-twitter )
endif ( NOT DISABLE_TWITTER)


if ( NOT DISABLE_VUNGLE )
  target_link_libraries ( moai-ios-host-modules moai-ios-vungle )
endif ( NOT DISABLE_VUNGLE)

set_target_properties( moai-ios-host-modules PROPERTIES
   XCODE_ATTRIBUTE_INSTALL_PATH "$(LOCAL_APPS_DIR)"
   XCODE_ATTRIBUTE_SKIP_INSTALL "NO"
   XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "5.1"
   XCODE_ATTRIBUTE_COMBINE_HIDPI_IMAGES "NO"
)


install(TARGETS moai-ios-host-modules EXPORT libmoai ARCHIVE DESTINATION lib )

install(FILES 
              ${MOAI_ROOT}/src/host-modules/aku_modules_ios.h 
              ${MOAI_ROOT}/src/host-modules/aku_modules_ios_config.h
        DESTINATION include/host-modules)

#for EXPORT
target_include_directories(moai-ios-host-modules INTERFACE "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>" )
