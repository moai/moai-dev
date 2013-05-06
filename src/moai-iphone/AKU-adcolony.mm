// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moaiext-iphone/AKU-adcolony.h>
#include <moaiext-iphone/MOAIAdColonyIOS.h>

//================================================================//
// AKU-adcolony
//================================================================//

//----------------------------------------------------------------//
void AKUAdColonyInit () {
	
	MOAIAdColonyIOS::Affirm();
	REGISTER_LUA_CLASS ( MOAIAdColonyIOS )
}