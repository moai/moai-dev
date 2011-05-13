//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <aku/aku.h>
#import <aku/aku-iphone.h>

#import "AppDelegate.h"
#import "MoaiView.h"

//================================================================//
// AppDelegate
//================================================================//
@implementation AppDelegate

	//----------------------------------------------------------------//
	-( void ) dealloc {

		[ mMoaiView release ];
		[ mWindow release ];
		[ super dealloc ];
	}

	//================================================================//
	#pragma mark -
	#pragma mark Protocol UIApplicationDelegate
	//================================================================//	

	//----------------------------------------------------------------//
	-( void ) application:( UIApplication* )application didFailToRegisterForRemoteNotificationsWithError:( NSError* )error {
	
		AKUAppDidFailToRegisterForRemoteNotificationsWithError ( error );
	}

	//----------------------------------------------------------------//
	-( BOOL ) application:( UIApplication* )application didFinishLaunchingWithOptions:( NSDictionary* )launchOptions {
		
		[ application setStatusBarHidden:YES ];
		
		[ mWindow makeKeyAndVisible ];
		
		[[ NSFileManager defaultManager ] changeCurrentDirectoryPath:[[ NSBundle mainBundle ] resourcePath ]];
		
		AKUIphoneInit ( application );
		
		// set up iphone specific app state
		[ mMoaiView run:@"iphone.lua" ];
		
		
		NSDictionary* userInfo = [ launchOptions objectForKey:UIApplicationLaunchOptionsRemoteNotificationKey ];
		if ( userInfo ) {
			AKUAppDidReceiveRemoteNotification ( userInfo );
		}
		
		UILocalNotification* notification = [ launchOptions objectForKey:UIApplicationLaunchOptionsLocalNotificationKey ];
		if ( notification ) {
			AKUAppDidReceiveLocalNotification ( notification );
		}
		
		// main setup
		[ mMoaiView run:@"main.lua" ];

		return YES;
	}
	
	//----------------------------------------------------------------//
	-( void ) application:( UIApplication* )application didReceiveLocalNotification:( UILocalNotification* )notification {
	
		AKUAppDidReceiveLocalNotification ( notification );
	}
	
	//----------------------------------------------------------------//
	-( void ) application:( UIApplication* )application didReceiveRemoteNotification:( NSDictionary* )userInfo {
		
		AKUAppDidReceiveRemoteNotification ( userInfo );
	}
	
	//----------------------------------------------------------------//
	-( void ) application:( UIApplication* )application didRegisterForRemoteNotificationsWithDeviceToken:( NSData* )deviceToken {
	
		AKUAppDidRegisterForRemoteNotificationsWithDeviceToken ( deviceToken );
	}
	
	//----------------------------------------------------------------//
	-( void ) applicationWillTerminate :( UIApplication* )application {

		AKUFinalize ();
	}

@end

