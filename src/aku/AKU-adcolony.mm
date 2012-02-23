// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <aku/AKU-adcolony.h>
#include <moaiext-iphone/MOAIAdColony.h>

//================================================================//
// AKU-adcolony
//================================================================//

//----------------------------------------------------------------//
void AKUAdColonyInit () {
	
	REGISTER_LUA_CLASS ( MOAIAdColony )
}

//----------------------------------------------------------------//
void AKUAdColonySetDelegate ( id < AdColonyDelegate > delegate ) {
	
	MOAIAdColony::Get ().SetAdColonyDelegate ( delegate );
}
