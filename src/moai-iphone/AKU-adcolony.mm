// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-iphone/AKU-adcolony.h>
#include <moai-iphone/MOAIAdColonyIOS.h>

//================================================================//
// AKU-adcolony
//================================================================//

//----------------------------------------------------------------//
void AKUAdColonyInit () {
	#ifndef DISABLE_ADCOLONY
	MOAIAdColonyIOS::Affirm();
	REGISTER_LUA_CLASS ( MOAIAdColonyIOS )
	#endif
}