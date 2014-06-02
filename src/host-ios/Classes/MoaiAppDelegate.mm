//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <host-modules/aku_modules_ios.h>

#import "MoaiAppDelegate.h"
#import "LocationObserver.h"
#import "MoaiVC.h"
#import "MoaiView.h"

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
	
		AKUIosNotifyRemoteNotificationRegistrationComplete ( nil );
	}

	//----------------------------------------------------------------//
	-( BOOL ) application:( UIApplication* )application didFinishLaunchingWithOptions:( NSDictionary* )launchOptions {
		
		[ application setStatusBarHidden:true ];
		
		AKUAppInitialize ();
		AKUModulesAppInitialize ();
		AKUModulesIosAppInitialize ();
		
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
		
         // check for launch with local notification
        NSDictionary* pushBundle = [ launchOptions objectForKey:UIApplicationLaunchOptionsLocalNotificationKey ];
        
        // if we have a local notification bundle, send that to the app...
        if ( pushBundle != NULL ) {
            AKUIosNotifyRemoteNotificationReceived ( pushBundle );
        } else {
            // ...else check for a remote bundle
            pushBundle = [ launchOptions objectForKey:UIApplicationLaunchOptionsRemoteNotificationKey ];
            if ( pushBundle != NULL ) {
                AKUIosNotifyRemoteNotificationReceived ( pushBundle );
            }
        }
		
		// return
		return true;
	}

	//----------------------------------------------------------------//
    -( void ) application:( UIApplication* )application didReceiveLocalNotification:( UILocalNotification* )notification {
    
        AKUIosNotifyLocalNotificationReceived ( notification );
    }

	//----------------------------------------------------------------//
	-( void ) application:( UIApplication* )application didReceiveRemoteNotification:( NSDictionary* )pushBundle {
		
		AKUIosNotifyRemoteNotificationReceived ( pushBundle );
	}
	
	//----------------------------------------------------------------//
	-( void ) application:( UIApplication* )application didRegisterForRemoteNotificationsWithDeviceToken:( NSData* )deviceToken {
	
		AKUIosNotifyRemoteNotificationRegistrationComplete ( deviceToken );
	}
	
	//----------------------------------------------------------------//
	-( void ) applicationDidBecomeActive:( UIApplication* )application {
	
		// restart moai view
		AKUIosDidBecomeActive ();
		[ mMoaiView pause:NO ];
	}

	//----------------------------------------------------------------//
	-( void ) applicationWillResignActive:( UIApplication* )application {
	
		// pause moai view
		AKUIosWillResignActive ();
		[ mMoaiView pause:YES ];
	}
	
	//----------------------------------------------------------------//
	-( void ) applicationWillTerminate :( UIApplication* )application {
        
		AKUIosWillTerminate ();
		AKUModulesIosAppFinalize ();
		AKUModulesAppFinalize ();
		AKUAppFinalize ();
	}

	//----------------------------------------------------------------//
	#if __IPHONE_OS_VERSION_MIN_REQUIRED > __IPHONE_4_1
		
		//----------------------------------------------------------------//
		// For iOS 4.2+ support
		-( BOOL )application:( UIApplication* )application openURL:( NSURL* )url sourceApplication:( NSString* )sourceApplication annotation:( id )annotation {

			AKUIosOpenUrl ( url, [ sourceApplication UTF8String ]);
			return YES;
		}
	
	#else

		//----------------------------------------------------------------//
		-( BOOL )application :( UIApplication* )application handleOpenURL :( NSURL* )url {

			AKUIosOpenUrl ( url, 0 );
			return YES;
		}

	#endif

@end
