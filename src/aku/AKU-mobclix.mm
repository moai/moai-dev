// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <aku/AKU-mobclix.h>
#include <moaiext-iphone/MOAIMobclixIOS.h>

//================================================================//
// AKU-mobclix
//================================================================//

//----------------------------------------------------------------//
void AKUMobclixInit () {
	
	MOAIMobclixIOS::Affirm();
	REGISTER_LUA_CLASS ( MOAIMobclixIOS );
}