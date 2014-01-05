// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-harness/host.h>
#include <moai-harness/MOAIHarness.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUHarnessAppFinalize () {
}

//----------------------------------------------------------------//
void AKUHarnessAppInitialize () {

	// disable buffering on stdout so that piped output gets flushed immediately
	setvbuf ( stdout, NULL, _IONBF, 0 );
}

//----------------------------------------------------------------//
void AKUHarnessContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIHarness )

	// Hook lua debug callbacks here
	lua_State* L = AKUGetLuaState ();
	MOAIHarness::Get().HookLua ( L, "127.0.0.1", 7018 );
}

//----------------------------------------------------------------//
void AKUHarnessUpdate () {
	lua_State* L = AKUGetLuaState ();
	MOAIHarness::Get ().Update ( L );
}