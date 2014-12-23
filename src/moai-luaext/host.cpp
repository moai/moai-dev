// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-luaext/host.h>

extern "C" {	
	extern int luaopen_crypto			( lua_State *L );
	extern int luaopen_lfs				( lua_State *L );
	//extern int luaopen_luacurl			( lua_State *L );
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

	#if MOAI_WITH_LIBCRYPTO
		luaopen_crypto ( state );
	#endif

  #if MOAI_WITH_CURL
		luaopen_luacurl ( state );
	#endif

  #if MOAI_WITH_SQLITE
  	luaopen_luasql_sqlite3 ( state );
  #endif
	
	luaopen_lfs ( state );
	luapreload_fullluasocket ( state );

}
