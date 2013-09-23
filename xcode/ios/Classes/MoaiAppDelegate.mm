//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <host-modules/aku_modules.h>
#import <moai-iphone/AKU-iphone.h>

#import "MoaiAppDelegate.h"
#import "LocationObserver.h"
#import "MoaiVC.h"
#import "MoaiView.h"

#if MOAI_WITH_BOX2D
	#include <moai-box2d/host.h>
#endif

#if MOAI_WITH_CHIPMUNK
	#include <moai-chipmunk/host.h>
#endif

//================================================================//
// AppDelegate
//================================================================//
@implementation MoaiAppDelegate

	@synthesize window = mWindow;
	@synthesize rootViewController = mMoaiVC;

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
	
		AKUNotifyRemoteNotificationRegistrationComplete ( nil );
	}

	//----------------------------------------------------------------//
	-( BOOL ) application:( UIApplication* )application didFinishLaunchingWithOptions:( NSDictionary* )launchOptions {
		
		[ application setStatusBarHidden:true ];
		
		AKUAppInitialize ();
		
		mMoaiView = [[ MoaiView alloc ] initWithFrame:[ UIScreen mainScreen ].bounds ];
		[ mMoaiView setUserInteractionEnabled:YES ];
		[ mMoaiView setMultipleTouchEnabled:YES ];
		[ mMoaiView setOpaque:YES ];
		[ mMoaiView setAlpha:1.0f ];

		mMoaiVC = [[ MoaiVC alloc ]	init ];
		[ mMoaiVC setView:mMoaiView ];
		
		mWindow = [[ UIWindow alloc ] initWithFrame:[ UIScreen mainScreen ].bounds ];
		[ mWindow setUserInteractionEnabled:YES ];
		[ mWindow setMultipleTouchEnabled:YES ];
		[ mWindow setOpaque:YES ];
		[ mWindow setAlpha:1.0f ];
		[ mWindow addSubview:mMoaiView ];
		[ mWindow setRootViewController:mMoaiVC ];
		[ mWindow makeKeyAndVisible ];
        
		[ mMoaiView moaiInit:application ];
		
		// select product folder
		NSString* luaFolder = [[[ NSBundle mainBundle ] resourcePath ] stringByAppendingString:@"/lua" ];
		AKUSetWorkingDirectory ([ luaFolder UTF8String ]);
		
		// run scripts
		[ mMoaiView run:@"main.lua" ];
		
        // check to see if the app was lanuched from a remote notification
        NSDictionary* pushBundle = [ launchOptions objectForKey:UIApplicationLaunchOptionsRemoteNotificationKey ];
        if ( pushBundle != NULL ) {
            
            AKUNotifyRemoteNotificationReceived ( pushBundle );
        }
		
		// return
		return true;
	}

		
	//----------------------------------------------------------------//
	-( void ) application:( UIApplication* )application didReceiveRemoteNotification:( NSDictionary* )pushBundle {
		
		AKUNotifyRemoteNotificationReceived ( pushBundle );
	}
	
	//----------------------------------------------------------------//
	-( void ) application:( UIApplication* )application didRegisterForRemoteNotificationsWithDeviceToken:( NSData* )deviceToken {
	
		AKUNotifyRemoteNotificationRegistrationComplete ( deviceToken );
	}
	
	//----------------------------------------------------------------//
	-( void ) applicationDidBecomeActive:( UIApplication* )application {
	
		// restart moai view
		AKUAppDidStartSession ( true );
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
		AKUAppWillEndSession ();
		[ mMoaiView pause:YES ];
	}
	
	//----------------------------------------------------------------//
	-( void ) applicationWillTerminate :( UIApplication* )application {
        
		AKUAppWillEndSession ();
		AKUAppFinalize ();
	}

	//----------------------------------------------------------------//
	#if __IPHONE_OS_VERSION_MIN_REQUIRED > __IPHONE_4_1
		
		//----------------------------------------------------------------//
		// For iOS 4.2+ support
		-( BOOL )application:( UIApplication* )application openURL:( NSURL* )url sourceApplication:( NSString* )sourceApplication annotation:( id )annotation {

			AKUAppOpenFromURL ( url );
			return YES;
		}
	
	#else

		//----------------------------------------------------------------//
		-( BOOL )application :( UIApplication* )application handleOpenURL :( NSURL* )url {

			AKUAppOpenFromURL(url);
			return YES;
		}

	#endif

@end
