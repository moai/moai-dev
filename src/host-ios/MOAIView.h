//----------------------------------------------------------------//
// Copyright (c) 2014 Plumzi, Inc.
// All Rights Reserved.
//----------------------------------------------------------------//

#import "MOAIOpenGLView.h"

//================================================================//
// PZMoaiView
//================================================================//
@interface MOAIView : MOAIOpenGLView <UIAccelerometerDelegate> {
@private
}

    //----------------------------------------------------------------//
    +( void )       appFinalize;
    +( void )       appInitialize;
	-( void )       application                             :( UIApplication* )application didFailToRegisterForRemoteNotificationsWithError:( NSError* )error;
	-( void )       application                             :( UIApplication* )application didReceiveRemoteNotification:( NSDictionary* )pushBundle;
	-( void )       application                             :( UIApplication* )application didRegisterForRemoteNotificationsWithDeviceToken:( NSData* )deviceToken;
	-( void )       applicationDidBecomeActive              :( UIApplication* )application;
	-( void )       applicationDidEnterBackground           :( UIApplication* )application;
	-( void )       applicationWillEnterForeground          :( UIApplication* )application;
	-( void )       applicationWillResignActive             :( UIApplication* )application;
	-( void )       applicationWillTerminate                :( UIApplication* )application;
    -( BOOL )       application                             :( UIApplication* )application openURL:( NSURL* )url sourceApplication:( NSString* )sourceApplication annotation:( id )annotation;
    +( CGRect )     getScreenBoundsFromCurrentOrientation   :( CGRect )bounds;
    +( BOOL )       isSystemVersionLessThan                 :( NSString* )version;
    -( void )       moaiInit                                :( UIApplication* )application;
    -( void )       pause                                   :( BOOL )paused;
    -( void )       run                                     :( NSString* )filename;

@end
