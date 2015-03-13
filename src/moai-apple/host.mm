// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#import "host.h"
#import "headers.h"

//================================================================//
// aku-util
//================================================================//

//----------------------------------------------------------------//
void AKUAppleAppFinalize () {
}

//----------------------------------------------------------------//
void AKUAppleAppInitialize () {

	loadMoaiLib_NSArray ();
	loadMoaiLib_NSData ();
	loadMoaiLib_NSDate ();
	loadMoaiLib_NSDictionary ();
	loadMoaiLib_NSError ();
	loadMoaiLib_NSNumber ();
	loadMoaiLib_NSObject ();
	loadMoaiLib_NSString ();
}

//----------------------------------------------------------------//
void AKUAppleContextInitialize () {
	
	MOAIReachabilityMgr::Affirm ();
	
	MOAIReachabilityMgr::Get ().UpdateReachability ();
}
