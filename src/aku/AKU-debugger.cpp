// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <aku/AKU-debugger.h>
#include <moaiext-debugger/MOAIHarness.h>

//================================================================//
// AKU-debugger
//================================================================//

//----------------------------------------------------------------//
void AKUDebugHarnessHandleError ( const char* message, struct lua_State* L, int level ) {
    MOAIHarness::Get().HandleError ( message, L, level );
}

//----------------------------------------------------------------//
void AKUDebugHarnessInit () {
	
	REGISTER_LUA_CLASS ( MOAIHarness )

	lua_State* L = AKUGetLuaState ();
	
	// Hook lua debug callbacks here
	MOAIHarness::Get().HookLua(L, "127.0.0.1", 7018);
	
	// disable buffering on stdout so that piped output gets flushed immediately
	setvbuf(stdout, NULL, _IONBF, 0);
}

//----------------------------------------------------------------//
void AKUDebugHarnessUpdate () {
	lua_State* L = AKUGetLuaState ();
	MOAIHarness::Get().Update(L);
}