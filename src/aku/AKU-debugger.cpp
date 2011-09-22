// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <aku/AKU-debugger.h>
#include <moaiext-debugger/MOAIHarness.h>

//================================================================//
// AKU-debugger
//================================================================//

//----------------------------------------------------------------//
void AKUDebugHarnessInit () {
	
	lua_State* L = AKUGetLuaState ();
	
	// Hook lua debug callbacks here
	MOAIHarness::HookLua(L, "127.0.0.1", 7018);
}