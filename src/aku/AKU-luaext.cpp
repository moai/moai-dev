// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <aku/AKU-luaext.h>
#include <moaicore/moaicore.h>

extern "C" {
	#include <lcrypto.h>
	#include <luasocket.h>
	
	extern int luaopen_crypto ( lua_State *L );
}

//================================================================//
// AKU-fmod
//================================================================//

//----------------------------------------------------------------//
void AKUExtLoadLuacrypto () {

	lua_State* state = AKUGetLuaState ();
	luaopen_crypto ( state );
}

//----------------------------------------------------------------//
void AKUExtLoadLuasocket () {

	lua_State* state = AKUGetLuaState ();
	luaopen_socket_core ( state );
}
