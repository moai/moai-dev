//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import "MOAIAppDelegate.h"
#import "MOAIView.h"

//================================================================//
// AppDelegate
//================================================================//
@implementation MOAIAppDelegate

	@synthesize window = mWindow;
	@synthesize rootViewController = mMoaiVC;

	//----------------------------------------------------------------//
	-( void ) dealloc {
	}

	//================================================================//
	#pragma mark -
	#pragma mark Protocol UIApplicationDelegate
	//================================================================//	

	//----------------------------------------------------------------//
	-( void ) application:( UIApplication* )application didFailToRegisterForRemoteNotificationsWithError:( NSError* )error {

        [ mMoaiView application:application didFailToRegisterForRemoteNotificationsWithError:error ];
	}

	//----------------------------------------------------------------//
	-( BOOL ) application:( UIApplication* )application didFinishLaunchingWithOptions:( NSDictionary* )launchOptions {
		( void )application;
        
		[ application setStatusBarHidden:true ];
        
        // clear old notifications
        NSArray* scheduledNotifications = [ NSArray arrayWithArray:application.scheduledLocalNotifications ];
        application.scheduledLocalNotifications = scheduledNotifications;

        [ MOAIView appInitialize ];
        
        CGRect nativeBounds = [[ UIScreen mainScreen ] bounds ];
        
        mMoaiView = [[ MOAIView alloc ] init ];
        mMoaiView.userInteractionEnabled = YES;
        mMoaiView.multipleTouchEnabled = YES;
        mMoaiView.alpha = 1.0f;
        mMoaiView.opaque = YES;
        
        mMoaiVC = [[ UIViewController alloc ] init ];
        mMoaiVC.view = mMoaiView;

        mWindow = [[ UIWindow alloc ] init ];
        [ mWindow makeKeyAndVisible ];
        mWindow.rootViewController = mMoaiVC;
        
        mWindow.frame = nativeBounds;
        mMoaiView.frame = nativeBounds;
        
        [ mMoaiView moaiInit ];
        [ mMoaiView setWorkingDirectory:[[ NSBundle mainBundle ] resourcePath ]];
        [ mMoaiView run:@"main.lua" ];
        
        // check to see if the app was lanuched from a remote notification
        // these keeps the old behavior, in which we 'fall back' on remote notifications
        // TODO: is this correct?
        NSDictionary* pushBundle = [ launchOptions objectForKey:UIApplicationLaunchOptionsLocalNotificationKey ];

        // fall back on remote notifications bundle
        if ( pushBundle == NULL ) {
            pushBundle = [ launchOptions objectForKey:UIApplicationLaunchOptionsRemoteNotificationKey ];
        }

        // if we have a bundle, handle it
        if ( pushBundle != NULL ) {
            [ mMoaiView application:application didReceiveRemoteNotification:pushBundle ];
        }

		// return
		return YES;
	}
	
    //----------------------------------------------------------------//
    -( void ) application:( UIApplication* )application didReceiveLocalNotification:( UILocalNotification* )notification {
    
        //[ mMoaiView application:application didReceiveLocalNotification:notification ];
    }   

	//----------------------------------------------------------------//
	-( void ) application:( UIApplication* )application didReceiveRemoteNotification:( NSDictionary* )pushBundle {

        [ mMoaiView application:application didReceiveRemoteNotification:pushBundle ];
	}
	
     //----------------------------------------------------------------//
    -( void ) application:( UIApplication* )application didRegisterForRemoteNotificationsWithError:( NSError* )error {
        
        //[ mMoaiView application:application didRegisterForRemoteNotificationsWithError:error ];
    }

	//----------------------------------------------------------------//
	-( void ) application:( UIApplication* )application didRegisterForRemoteNotificationsWithDeviceToken:( NSData* )deviceToken {

        [ mMoaiView application:application didRegisterForRemoteNotificationsWithDeviceToken:deviceToken ];
	}

	//----------------------------------------------------------------//
	-( void ) applicationDidBecomeActive:( UIApplication* )application {

        [ mMoaiView pause:NO ];
	}

	//----------------------------------------------------------------//
	-( void ) applicationWillResignActive:( UIApplication* )application {

        [ mMoaiView pause:YES ];
	}
	
    //----------------------------------------------------------------//
    -( BOOL )application:( UIApplication* )application openURL:( NSURL* )url sourceApplication:( NSString* )sourceApplication annotation:( id )annotation {
        [ mMoaiView application:application openURL:url sourceApplication:sourceApplication annotation:annotation ];
        return YES;
    }

@end
