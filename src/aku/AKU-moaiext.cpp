// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <aku/AKU-moaiext.h>
#include <moaicore/moaicore.h>
#include <moaicore/MOAIFmod.h>
#include <moaicore/MOAIFmodChannel.h>
#include <moaicore/MOAIFmodSound.h>

#if MOAIEXT_USE_LUACRYPTO
	extern "C" {
		#include <lcrypto.h>
	}
#endif

#if MOAIEXT_USE_LUASOCKET
	extern "C" {
		#include <luasocket.h>
		extern int luaopen_crypto ( lua_State *L );
	}
#endif

//================================================================//
// AKU-fmod
//================================================================//

//----------------------------------------------------------------//
void AKUExtLoadFmod () {

	#if MOAIEXT_USE_FMOD

		MOAIFmod::Get ();
		
		REGISTER_LUA_CLASS ( MOAIFmod )
		REGISTER_LUA_CLASS ( MOAIFmodChannel )
		REGISTER_LUA_CLASS ( MOAIFmodSound )
	
	#endif
}

//----------------------------------------------------------------//
void AKUExtLoadLuacrypto () {

	#if MOAIEXT_USE_LUACRYPTO

		lua_State* state = AKUGetLuaState ();
		luaopen_crypto ( state );
	
	#endif
}

//----------------------------------------------------------------//
void AKUExtLoadLuasocket () {

	#if MOAIEXT_USE_LUASOCKET

		lua_State* state = AKUGetLuaState ();
		luaopen_socket_core ( state );
	
	#endif
}
