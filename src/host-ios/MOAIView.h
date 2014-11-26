//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

//================================================================//
// MOAIView
//================================================================//
@interface MOAIView : UIView {
@private
}
    @property ( readonly, nonatomic ) int multisample;
    @property ( readonly, nonatomic ) BOOL multisampleEnabled;

    //----------------------------------------------------------------//
    +( void )       appInitialize;
	-( void )       application                             :( UIApplication* )application didFailToRegisterForRemoteNotificationsWithError:( NSError* )error;
	-( void )       application                             :( UIApplication* )application didReceiveRemoteNotification:( NSDictionary* )pushBundle;
	-( void )       application                             :( UIApplication* )application didRegisterForRemoteNotificationsWithDeviceToken:( NSData* )deviceToken;
    -( BOOL )       application                             :( UIApplication* )application openURL:( NSURL* )url sourceApplication:( NSString* )sourceApplication annotation:( id )annotation;
    +( CGRect )     getScreenBoundsFromCurrentOrientation   :( CGRect )bounds;
    +( BOOL )       isSystemVersionLessThan                 :( NSString* )version;
    -( void )       moaiInit;
    -( void )       moaiInitWithMultisample                 :( int )multisample; // pass 1 or 4 for multisample
    -( void )       pause                                   :( BOOL )paused;
    -( void )       run                                     :( NSString* )filename;
    -( void )       setWorkingDirectory                     :( NSString* )path;

@end
