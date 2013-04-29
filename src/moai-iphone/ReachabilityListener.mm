// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#import <ReachabilityListener.h>
#import <Reachability.h>
#import <aku/AKU-iphone.h>

//================================================================//
// ReachabilityListener
//================================================================//
@implementation ReachabilityListener


//----------------------------------------------------------------//
-(void) reachabilityChanged: (NSNotification* )note {

	Reachability* curReach = [ note object ];
	
    AKUSetConnectionType ([ curReach currentReachabilityStatus ]);
}


//----------------------------------------------------------------//
- ( void ) startListener {

	[[ NSNotificationCenter defaultCenter ] addObserver: self selector: @selector ( reachabilityChanged: ) name: kReachabilityChangedNotification object: nil ];
	mReach = [[ Reachability reachabilityForInternetConnection ] retain ];
	[ mReach startNotifier ];
}


//----------------------------------------------------------------//
- ( void ) stopListener {	
	[ mReach release ];
}

@end
