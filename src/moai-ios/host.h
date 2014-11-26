//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef MOAI_IOS_HOST_H
#define MOAI_IOS_HOST_H

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

#include <moai-core/host.h>

AKU_API void	AKUIosAppFinalize									();
AKU_API void	AKUIosAppInitialize									();
AKU_API void	AKUIosContextInitialize								();

AKU_API void	AKUIosNotifyLocalNotificationReceived				( UILocalNotification* notification );
AKU_API void	AKUIosNotifyRemoteNotificationReceived				( NSDictionary* notification );
AKU_API void	AKUIosNotifyRemoteNotificationRegistrationComplete	( NSData* deviceToken, NSError* error );
AKU_API void	AKUIosOpenUrl										( NSURL* url, NSString* sourceApplication );
AKU_API void	AKUIosSetFrameBuffer								( GLuint frameBuffer );

#endif
