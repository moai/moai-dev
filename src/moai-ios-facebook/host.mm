// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-ios-facebook/host.h>
#include <moai-ios-facebook/MOAIFacebookIOS.h>

#import <FBSDKCoreKit/FBSDKCoreKit.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUIosFacebookAppFinalize () {
}

//----------------------------------------------------------------//
void AKUIosFacebookAppInitialize () {
}

//----------------------------------------------------------------//
BOOL AKUIosFacebookApplicationDidFinishLaunchingWithOptions ( UIApplication* application, NSDictionary* launchOptions ) {

	[[ FBSDKApplicationDelegate sharedInstance ] application:application didFinishLaunchingWithOptions:launchOptions ];
	return YES;
}

//----------------------------------------------------------------//
BOOL AKUIosFacebookApplicationOpenURL ( UIApplication* application,  NSURL* url, NSString* sourceApplication, id annotation ) {

	return [[ FBSDKApplicationDelegate sharedInstance ] application:application openURL:url sourceApplication:sourceApplication annotation:annotation ];
}

//----------------------------------------------------------------//
void AKUIosFacebookContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIFacebookIOS );
}
