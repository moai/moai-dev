//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@class MOAIView;
@class MOAIViewController;

//================================================================//
// MOAIAppDelegate
//================================================================//
@interface MOAIAppDelegate : NSObject < UIApplicationDelegate > {
@private

	MOAIView*               mMoaiView;
	UIWindow*               mWindow;
    UIImageView*            mSplashView;
	MOAIViewController*		mMoaiVC;
    BOOL                    mResumed;
}

@property ( nonatomic, retain ) UIWindow* window;
@property ( nonatomic, retain ) UIViewController* rootViewController;

@end
