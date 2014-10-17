// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com
#import <Foundation/Foundation.h>

@class MOAIReachability;
class MOAIGlobals;

//================================================================//
// MOAIReachabilityListener
//================================================================//
@interface MOAIReachabilityListener : NSObject {

	MOAIGlobals*			mContext;
	MOAIReachability*		mReach;
}

	//----------------------------------------------------------------//
	-( void )	startListener;
	-( void )	stopListener;
	-( void )	updateMoaiEnvironment;

@end
