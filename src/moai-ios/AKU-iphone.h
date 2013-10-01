//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef AKU_IPHONE_H
#define AKU_IPHONE_H

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>
#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

//#define DISABLE_TAPJOY

enum ConnectionType {
	CONNECTION_TYPE_NONE,
	CONNECTION_TYPE_WIFI,
	CONNECTION_TYPE_WWAN
};

void			AKUAppDidStartSession							( bool resumed );
void			AKUAppOpenFromURL								( NSURL* url );
void			AKUAppWillEndSession							();
const char*		AKUGetGUID										();
long			AKUGetIphoneNetworkReachability					();
void			AKUIphoneInit									( UIApplication* application );
void			AKUNotifyLocalNotificationReceived				( UILocalNotification* notification );
void			AKUNotifyRemoteNotificationReceived				( NSDictionary* notification );
void			AKUNotifyRemoteNotificationRegistrationComplete	( NSData* deviceToken );
void			AKUSetConnectionType							( long type );
void			AKUViewDidRotateFromInterfaceOrientation		( UIInterfaceOrientation orientation );
void			AKUSetFrameBuffer								( GLuint frameBuffer );

#endif
