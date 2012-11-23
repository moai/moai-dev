// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <aku/AKU-smartfox.h>
#include <moaiext-iphone/MOAISmartFoxIOS.h>

//================================================================//
// AKU-chartboost
//================================================================//

//----------------------------------------------------------------//
void AKUSmartFoxInit () {
	
	MOAISmartFoxIOS::Affirm();
	REGISTER_LUA_CLASS ( MOAISmartFoxIOS );
}