//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <aku/aku.h>
#import <aku/aku-iphone.h>

#import "AppDelegate.h"
#import "LocationObserver.h"
#import "MoaiView.h"

//================================================================//
// AppDelegate
//================================================================//
@implementation AppDelegate

	//----------------------------------------------------------------//
	-( void ) dealloc {

		[ mMoaiVC release ];
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

		// init aku
		AKUIphoneInit ( application );
	
		// configure window
		[ application setStatusBarHidden:true ];
		[ mWindow addSubview:mMoaiView ];
		[ mWindow makeKeyAndVisible ];

		// select product folder
		[[ NSFileManager defaultManager ] changeCurrentDirectoryPath:[[[ NSBundle mainBundle ] resourcePath ] stringByAppendingString:@"/lua" ]];
		
		// run scripts
		[ mMoaiView run:@"main.lua" ];

		// return
		return true;
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
	-( void ) applicationDidBecomeActive:( UIApplication* )application {
	
		// restart moai view
		[ mMoaiView pause:NO ];
	}
	
	//----------------------------------------------------------------//
	-( void ) applicationDidEnterBackground:( UIApplication* )application {
	}
	
	//----------------------------------------------------------------//
	-( void ) applicationWillEnterForeground:( UIApplication* )application {
	}
	
	//----------------------------------------------------------------//
	-( void ) applicationWillResignActive:( UIApplication* )application {
	
		// pause moai view
		[ mMoaiView pause:YES ];
	}
	
	//----------------------------------------------------------------//
	-( void ) applicationWillTerminate :( UIApplication* )application {

		AKUFinalize ();
	}

@end
