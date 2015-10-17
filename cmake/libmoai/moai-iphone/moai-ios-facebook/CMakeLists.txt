cmake_minimum_required ( VERSION 2.8.11 )
project ( moai-ios-facebook )

#lib

# <FBSDKCoreKit/FBSDKCoreKit.h>
# <FBSDKLoginKit/FBSDKLoginKit.h>
# <FBSDKShareKit/FBSDKShareKit.h>

set(FACEBOOK_ROOT ${MOAI_ROOT}/3rdparty/facebook-ios-sdk-4.5.1)
# set(FACEBOOK_ROOT ${MOAI_ROOT}/3rdparty/facebook-ios-sdk-3.14.1/FacebookSDK.framework)

set(FACEBOOK_CORE_ROOT ${FACEBOOK_ROOT}/FBSDKCoreKit.framework)
set(FACEBOOK_LOGIN_ROOT ${FACEBOOK_ROOT}/FBSDKLoginKit.framework)
set(FACEBOOK_SHARE_ROOT ${FACEBOOK_ROOT}/FBSDKShareKit.framework)

set(FACEBOOK_CORE_LIB ${FACEBOOK_CORE_ROOT}/FBSDKCoreKit)
set(FACEBOOK_LOGIN_LIB ${FACEBOOK_LOGIN_ROOT}/FBSDKLoginKit)
set(FACEBOOK_SHARE_LIB ${FACEBOOK_SHARE_ROOT}/FBSDKShareKit)

# set(FACEBOOK_LIB ${FACEBOOK_ROOT}/FacebookSDK)

add_library(FacebookCoreKit SHARED IMPORTED)
set_property(TARGET FacebookCoreKit PROPERTY IMPORTED_LOCATION ${FACEBOOK_CORE_LIB})
set_property(TARGET FacebookCoreKit PROPERTY FRAMEWORK 1)

add_library(FacebookLoginKit SHARED IMPORTED)
set_property(TARGET FacebookLoginKit PROPERTY IMPORTED_LOCATION ${FACEBOOK_LOGIN_LIB})
set_property(TARGET FacebookLoginKit PROPERTY FRAMEWORK 1)

add_library(FacebookShareKit SHARED IMPORTED)
set_property(TARGET FacebookShareKit PROPERTY IMPORTED_LOCATION ${FACEBOOK_SHARE_LIB})
set_property(TARGET FacebookShareKit PROPERTY FRAMEWORK 1)

install(DIRECTORY "${FACEBOOK_CORE_ROOT}" DESTINATION lib)
install(DIRECTORY "${FACEBOOK_LOGIN_ROOT}" DESTINATION lib)
install(DIRECTORY "${FACEBOOK_SHARE_ROOT}" DESTINATION lib)

#glue
set (CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-ios-facebook")
file ( GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.mm" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )


add_library(moai-ios-facebook STATIC  ${SRC})

target_include_directories(moai-ios-facebook PUBLIC
		"$<BUILD_INTERFACE:${MOAI_ROOT}/src>"
        "$<BUILD_INTERFACE:${FACEBOOK_CORE_ROOT}>"
		"$<BUILD_INTERFACE:${FACEBOOK_LOGIN_ROOT}>"
        "$<BUILD_INTERFACE:${FACEBOOK_SHARE_ROOT}>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
	)

target_link_libraries( moai-ios-facebook moai-sim moai-util moai-core FacebookCoreKit FacebookLoginKit FacebookShareKit sqlite3)
set_target_properties( moai-ios-facebook PROPERTIES FOLDER Modules)
install(TARGETS moai-ios-facebook EXPORT libmoai ARCHIVE DESTINATION lib)
install(FILES ${MOAI_ROOT}/src/moai-ios-facebook/host.h DESTINATION include/moai-ios-facebook)
