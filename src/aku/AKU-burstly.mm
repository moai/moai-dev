// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <aku/AKU-burstly.h>
#include <moaiext-iphone/MOAIBurstlyIOS.h>

//================================================================//
// AKU-chartboost
//================================================================//

//----------------------------------------------------------------//
void AKUBurstlyInit () {
	
	MOAIBurstlyIOS::Affirm();
	REGISTER_LUA_CLASS ( MOAIBurstlyIOS );
}