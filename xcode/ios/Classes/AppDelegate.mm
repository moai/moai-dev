//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <aku/AKU.h>
#import <aku/AKU-iphone.h>

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
	
		AKUNotifyRemoteNotificationRegistrationComplete ( nil );
	}

	//----------------------------------------------------------------//
	-( BOOL ) application:( UIApplication* )application didFinishLaunchingWithOptions:( NSDictionary* )launchOptions {

		// configure window
		[ application setStatusBarHidden:true ];
		mWindow.rootViewController = ( UIViewController* )mMoaiVC;
		[ mWindow addSubview:mMoaiView ];
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

  //----------------------------------------------------------------//
#if __IPHONE_OS_VERSION_MIN_REQUIRED > __IPHONE_4_1
  // For iOS 4.2+ support
  //----------------------------------------------------------------//
  - (BOOL)application:(UIApplication *)application 
              openURL:(NSURL *)url
    sourceApplication:(NSString *)sourceApplication 
           annotation:(id)annotation {

      AKUAppOpenFromURL(url);

    return YES;
  }
#else

  //----------------------------------------------------------------//
  - (BOOL)application :( UIApplication* )application handleOpenURL :( NSURL* )url {

    AKUAppOpenFromURL(url);
    return YES;

  }

#endif

@end
