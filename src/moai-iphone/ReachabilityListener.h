// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com
#import <Foundation/Foundation.h>

@class Reachability;

//================================================================//
// ReachabilityListener
//================================================================//
@interface ReachabilityListener : NSObject {

	Reachability*		mReach;
	
}

	//----------------------------------------------------------------//
	-( void )	startListener;

@end
