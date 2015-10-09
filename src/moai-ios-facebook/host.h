//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef AKU_IOS_FACEBOOK_HOST_H
#define AKU_IOS_FACEBOOK_HOST_H

#include <moai-core/host.h>

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

AKU_API void	AKUIosFacebookAppFinalize								();
AKU_API void	AKUIosFacebookAppInitialize								();
AKU_API BOOL	AKUIosFacebookApplicationDidFinishLaunchingWithOptions	( UIApplication* application, NSDictionary* launchOptions );
AKU_API BOOL	AKUIosFacebookApplicationOpenURL						( UIApplication* application,  NSURL* url, NSString* sourceApplication, id annotation );
AKU_API void	AKUIosFacebookContextInitialize							();

#endif
