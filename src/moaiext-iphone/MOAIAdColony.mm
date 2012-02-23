//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <moaiext-iphone/MOAIAdColony.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIAdColony::_initAdColony ( lua_State* L ) {
	MOAILuaState state ( L );
     
	[ AdColony initAdColonyWithDelegate:MOAIAdColony::Get ().mAdColonyDelegate ];    
    
	return 0;
}

//================================================================//
// MOAICrittercism
//================================================================//

//----------------------------------------------------------------//
MOAIAdColony::MOAIAdColony () {
    
	RTTI_SINGLE ( MOAILuaObject )	
}

//----------------------------------------------------------------//
MOAIAdColony::~MOAIAdColony () {
    
}

//----------------------------------------------------------------//
void MOAIAdColony::RegisterLuaClass ( MOAILuaState& state ) {
    
	luaL_Reg regTable[] = {
		{ "initAdColony",		_initAdColony },
		{ NULL, NULL }
	};
    
	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAdColony::SetAdColonyDelegate ( id < AdColonyDelegate > delegate ) {
	
	mAdColonyDelegate = delegate;
}