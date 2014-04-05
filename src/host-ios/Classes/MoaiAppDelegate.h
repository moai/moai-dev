//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import "RefPtr.h"

@class MoaiVC;
@class MoaiView;

//================================================================//
// MoaiAppDelegate
//================================================================//
@interface MoaiAppDelegate : NSObject < UIApplicationDelegate > {
@private

	MoaiView*	mMoaiView;
	UIWindow*	mWindow;	
	MoaiVC*		mMoaiVC;
}

@property ( nonatomic, retain ) UIWindow* window;
@property ( nonatomic, retain ) UIViewController* rootViewController;

@end
