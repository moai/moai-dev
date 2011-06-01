// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <aku/AKU-luasocket.h>

extern "C" {
	#include <luasocket.h>
}

//================================================================//
// AKU-luasocket
//================================================================//

//----------------------------------------------------------------//
void AKULoadLuasocket () {

	lua_State* state = AKUGetLuaState ();
	luaopen_socket_core ( state );
}
