//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import "RefPtr.h"

@class MoaiView;

//================================================================//
// AppDelegate
//================================================================//
@interface AppDelegate : NSObject < UIApplicationDelegate > {
@private
	
	IBOutlet MoaiView*	mMoaiView;
	IBOutlet UIWindow*	mWindow;
}

@end
