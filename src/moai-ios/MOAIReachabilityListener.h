// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com
#import <Foundation/Foundation.h>

@class MOAIReachability;

//================================================================//
// MOAIReachabilityListener
//================================================================//
@interface MOAIReachabilityListener : NSObject {

	MOAIReachability*		mReach;
}

	//----------------------------------------------------------------//
	-( void )	startListener;
	+( void )	updateMoaiEnvironment;

@end
