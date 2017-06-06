//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <moai-ios-tenjin/MOAITenjinIOS.h>
#import <TenjinSDK.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	init
	@text	Initialize Tenjin.
	
	@in		string	apikey			Available in Tenjin dashboard settings.
	@out 	nil
*/
int MOAITenjinIOS::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* apikey = lua_tostring ( state, 1 );
	
	[TenjinSDK sharedInstanceWithToken:[ NSString stringWithUTF8String:apikey ]];

	return 0;
}

//================================================================//
// MOAITenjinIOS
//================================================================//

//----------------------------------------------------------------//
MOAITenjinIOS::MOAITenjinIOS () {

	RTTI_SINGLE ( MOAILuaObject )	
}

//----------------------------------------------------------------//
MOAITenjinIOS::~MOAITenjinIOS () {
}

//----------------------------------------------------------------//
void MOAITenjinIOS::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getListener",			&MOAIGlobalEventSource::_getListener < MOAITenjinIOS > },
		{ "init",					_init },
		{ "setListener",			&MOAIGlobalEventSource::_setListener < MOAITenjinIOS > },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
