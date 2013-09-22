// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-luaext/host.h>

extern "C" {	
	extern int luaopen_crypto			( lua_State *L );
	extern int luaopen_lfs				( lua_State *L );
	extern int luaopen_luacurl			( lua_State *L );
	extern int luaopen_luasql_sqlite3	( lua_State *L );
	extern int luapreload_fullluasocket ( lua_State *L );
}

//================================================================//
// aku-luaext
//================================================================//

//----------------------------------------------------------------//
void AKULuaExtAppFinalize () {
}

//----------------------------------------------------------------//
void AKULuaExtAppInitialize () {
}

//----------------------------------------------------------------//
void AKULuaExtContextInitialize () {

	lua_State* state = AKUGetLuaState ();
	luaopen_crypto ( state );
	luaopen_luacurl ( state );
	luaopen_lfs ( state );
	luapreload_fullluasocket ( state );
	luaopen_luasql_sqlite3 ( state );
}
