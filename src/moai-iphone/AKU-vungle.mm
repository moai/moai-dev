// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-iphone/AKU-vungle.h>
#include <moai-iphone/MOAIVungleIOS.h>

//================================================================//
// AKU-vungle
//================================================================//

//----------------------------------------------------------------//
void AKUVungleInit () {
	
	MOAIVungleIOS::Affirm();
	REGISTER_LUA_CLASS ( MOAIVungleIOS );
}