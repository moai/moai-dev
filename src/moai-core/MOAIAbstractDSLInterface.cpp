// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIAbstractDSLInterface.h>
#include <moai-core/MOAIAbstractDSLStream.h>

//================================================================//
// MOAIDSLInterfaceMgr
//================================================================//
class MOAIDSLInterfaceMgr :
	public ZLContextClass < MOAIDSLInterfaceMgr > {
public:

	int		mRefID;

public:

	//----------------------------------------------------------------//
	MOAIDSLInterfaceMgr () :
		mRefID ( LUA_NOREF ) {
	}
};

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractDSLInterface::_ ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDSLStream, "U" )
	
	MOAIAbstractDSLInterface* handler = NULL;
	
	if ( lua_istable ( state, 2 )) {
		handler = MOAILuaRuntime::Get ().GetSingletonForClass < MOAIAbstractDSLInterface >( state, 2 );
	}
	else {
		handler = state.GetLuaObject < MOAIAbstractDSLInterface >( 2, false );
		if ( handler ) {
			self->RetainObject ( handler );
		}
	}
	if ( handler ) {
		self->SelectAPI ( handler );
		self->PushInterface ( state );
		return 1;
	}
	return 0;
}

//================================================================//
// MOAIAbstractDSLInterface
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractDSLInterface::InitializeBinding ( MOAIAbstractDSLStream& stream ) const {
	return this->MOAIAbstractDSLInterface_InitializeBinding ( stream );
}

//----------------------------------------------------------------//
MOAIAbstractDSLInterface::MOAIAbstractDSLInterface () {

	RTTI_BEGIN ( MOAIAbstractDSLInterface )
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractDSLInterface::~MOAIAbstractDSLInterface () {
}

//----------------------------------------------------------------//
void MOAIAbstractDSLInterface::PushLuaAPI ( ZLTypeID apiTypeID, MOAILuaState& state ) {

	ZLType::RAW_ID rawAPITypeID = ZLType::ToRaw ( apiTypeID );

	MOAIDSLInterfaceMgr::Affirm ();
	MOAIDSLInterfaceMgr& mgr = MOAIDSLInterfaceMgr::Get ();

	// affirm the table of interfaces
	if ( mgr.mRefID == LUA_NOREF ) {
		lua_newtable ( state );
		mgr.mRefID = luaL_ref ( state, LUA_REGISTRYINDEX );
	}

	// get the interface
	lua_rawgeti ( state, LUA_REGISTRYINDEX, mgr.mRefID ); // table of interfaces
	
	lua_pushinteger ( state, rawAPITypeID );
	lua_rawget ( state, -2 );
	
	// if we have the interface, replace the registry and return
	if ( lua_istable ( state, -1 )) {
		lua_replace ( state, -2 );
		return;
	}
	
	// pop off the nil value
	lua_pop ( state, 1 );
	
	// create and initialize the interface table
	lua_newtable ( state );
	this->RegisterLuaAPI ( state );
	
	// interface is its own metatable
	lua_pushstring ( state, "__index" );
	lua_pushvalue ( state, -2 );
	lua_rawset ( state, -3 );
	
	// store it for future use
	lua_pushinteger ( state, rawAPITypeID );
	lua_pushvalue ( state, -2 );
	lua_rawset ( state, -4 );
	
	// replace the registry and we're done
	lua_replace ( state, -2 );
}

//----------------------------------------------------------------//
void MOAIAbstractDSLInterface::RegisterLuaAPI ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "_",							_ },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
	this->MOAIAbstractDSLInterface_RegisterLuaAPI ( state );
}
