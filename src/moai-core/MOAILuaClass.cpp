// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAILua.h>
#include <moai-core/MOAISerializer.h>

//================================================================//
// MOAILuaClass
//================================================================//

//----------------------------------------------------------------//
int MOAILuaClass::_extendFactory ( lua_State* L ) {

	MOAILuaState state ( L );

	// upvalues:
	// 1: class table
	// 2: interface table

	// clone the class table
	state.CloneTable ( lua_upvalueindex ( 1 ));
	
	// add getClassName to class table
	lua_pushvalue ( L, 1 );
	lua_pushcclosure ( L, _getUpvalue, 1 );
	lua_setfield ( L, -2, "getClassName" );
	
	// clone the interface table
	state.CloneTable ( lua_upvalueindex ( 2 ));
	
	// set the interface table as its own __index
	lua_pushvalue ( state, -1 );
	lua_setfield ( state, -2, "__index" );
	
	// add getClass to interface table
	lua_pushvalue ( L, -2 );
	lua_pushcclosure ( L, _getUpvalue, 1 );
	lua_setfield ( L, -2, "getClass" );
	
	// add getClassName to interface table
	lua_pushvalue ( L, 1 );
	lua_pushcclosure ( L, _getUpvalue, 1 );
	lua_setfield ( L, -2, "getClassName" );
	
	// stack:
	// -1: interface table
	// -2: class table
	
	// copy the extended interface table
	lua_pushvalue ( L, -1 );

	// get and push the original factory method
	lua_pushvalue ( L, lua_upvalueindex ( 1 ));
	lua_getfield ( L, -1, "new" );
	lua_replace ( L, -2 );

	// push the 'new' function with the interface table and original factory method as upvalues
	lua_pushcclosure ( L, _new, 2 );
	
	// set the extended 'new' method into the class table
	lua_setfield ( L, -3, "new" );
	
	// stack:
	// -1: interface table
	// -2: class table
	
	// now copy the extended class and interface tables
	lua_pushvalue ( L, -2 );
	lua_pushvalue ( L, -2 );
	
	// push the 'extend' method with the extended class and interface tables as upvalues
	lua_pushcclosure ( L, _extendFactory, 2 );
	
	// set the extended 'extend' method into the class table
	lua_setfield ( L, -3, "extend" );
	
	// stack:
	// -1: interface table
	// -2: class table
	
	// init the getInterfaceTable method
	lua_pushvalue ( L, -1 );
	lua_pushcclosure ( state, _getInterfaceTable, 1 );
	lua_setfield ( state, -3, "getInterfaceTable" );
	
	// stack:
	// -1: interface table
	// -2: class table

	// call the extender
	if ( state.IsType ( 2, LUA_TFUNCTION )) {
		lua_pushvalue ( L, 2 ); // function
		lua_pushvalue ( L, -2 ); // interface table
		lua_pushvalue ( L, -4 ); // class table
		lua_pushvalue ( L, lua_upvalueindex ( 2 )); // super interface table
		lua_pushvalue ( L, lua_upvalueindex ( 1 )); // super class table
		state.DebugCall ( 4, 0 );
	}

	// stack:
	// -1: interface table
	// -2: class table
	
	lua_pop ( L, 1 );
	
	// stack:
	// -1: class table

	// and we're done
	cc8* classname = state.GetValue < cc8* >( 1, "" );
	lua_setglobal ( state, classname );
	return 0;
}

//----------------------------------------------------------------//
int MOAILuaClass::_extendSingleton ( lua_State* L ) {

	MOAILuaState state ( L );
	
	// upvalues:
	// 1: singleton userdata
	// 2: class table
	
	// set the userdata
	MOAILuaObject* luaData = ( MOAILuaObject* )state.GetPtrUserData ( lua_upvalueindex ( 1 ));
	state.PushPtrUserData ( luaData );
	
	// clone the class table
	state.CloneTable ( lua_upvalueindex ( 2 ));
	
	lua_pushvalue ( state, -1 );
	lua_setfield ( state, -2, "__index" );

	lua_pushvalue ( state, -1 );
	lua_setfield ( state, -2, "__newindex" );
	
	// add getClassName to class table
	lua_pushvalue ( L, 1 );
	lua_pushcclosure ( L, _getUpvalue, 1 );
	lua_setfield ( L, -2, "getClassName" );
	
	// copy the extended userdata
	lua_pushvalue ( L, -2 );
	
	// copy the extended table
	lua_pushvalue ( L, -2 );
	
	// push the 'extend' method with the singleton userdata and extended class table upvalues
	lua_pushcclosure ( L, _extendSingleton, 2 );
	
	// set the extended 'extend' method...
	lua_setfield ( L, -2, "extend" );

	// stack:
	// -1: extended class table
	// -2: extended userdata
	
	// call the extender
	if ( state.IsType ( 2, LUA_TFUNCTION )) {
		lua_pushvalue ( L, 2 );
		lua_pushvalue ( L, -2 );
		lua_pushvalue ( L, lua_upvalueindex ( 2 ));
		state.DebugCall ( 2, 0 );
	}
	
	// stack:
	// -1: extended class table
	// -2: extended userdata
	
	// set the table as a metatable on the userdata
	lua_setmetatable ( L, -2 );
	
	// and we're done
	cc8* classname = state.GetValue < cc8* >( 1, "" );
	lua_setglobal ( state, classname );
	return 0;
}

//----------------------------------------------------------------//
int MOAILuaClass::_getInterfaceTable ( lua_State* L ) {

	MOAILuaState state ( L );
	lua_pushvalue ( L, lua_upvalueindex ( 1 ));
	return 1;
}

//----------------------------------------------------------------//
int MOAILuaClass::_getUpvalue ( lua_State* L ) {

	lua_pushvalue ( L, lua_upvalueindex ( 1 ));
	return 1;
}

//----------------------------------------------------------------//
int MOAILuaClass::_new ( lua_State* L ) {

	// upvalues:
	// 1: interface table
	// 2: original 'new'

	MOAILuaState state ( L );
	
	lua_pushvalue ( L, lua_upvalueindex ( 2 ));
	if ( state.IsType ( -1, LUA_TFUNCTION )) {
	
		// call the original new
		state.DebugCall ( 0, 1 );
		
		if ( state.IsType ( -1, LUA_TUSERDATA )) {
		
			// get the ref table
			if ( lua_getmetatable ( state, -1 )) {
				
				// get the member table
				if ( lua_getmetatable ( state, -1 )) {
				
					// get the interface table
					lua_pushvalue ( L, lua_upvalueindex ( 1 ));
					
					// set the interface table as the metatable
					lua_setmetatable ( L, -2 );
					
					// done with the member table
					lua_pop ( L, 1 );
				}
				// done with the ref table
				lua_pop ( L, 1 );
			}
		}
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
MOAILuaObject* MOAILuaClass::GetSingleton () {
	return 0;
}

//----------------------------------------------------------------//
void MOAILuaClass::InitLuaFactoryClass ( MOAILuaObject& data, MOAILuaState& state ) {

	int top = lua_gettop ( state );

	// push interface table
	lua_newtable ( state );
	
	// interface table is its own __index
	lua_pushvalue ( state, -1 );
	lua_setfield ( state, -2, "__index" );
	
	data.MOAILuaObject::RegisterLuaFuncs ( state );
	data.RegisterLuaFuncs ( state );

	this->mInterfaceTable.SetRef ( state, -1 );
	
	// reset top
	lua_settop ( state, top );

	// push class table
	lua_newtable ( state );
	this->RegisterLuaClass ( state );
	data.MOAILuaObject::RegisterLuaClass ( state );
	data.RegisterLuaClass ( state );

	// init the extend method
	lua_pushvalue ( state, -1 ); // class table
	this->mInterfaceTable.PushRef ( state ); // interface table
	lua_pushcclosure ( state, _extendFactory, 2 );
	lua_setfield ( state, -2, "extend" );

	// init the getInterfaceTable method
	this->mInterfaceTable.PushRef ( state ); // interface table
	lua_pushcclosure ( state, _getInterfaceTable, 1 );
	lua_setfield ( state, -2, "getInterfaceTable" );

	// ref class table and expose as global
	this->mClassTable.SetRef ( state, -1 );
	lua_setglobal ( state, data.TypeName ());

	// reset top
	lua_settop ( state, top );
}

//----------------------------------------------------------------//
void MOAILuaClass::InitLuaSingletonClass ( MOAILuaObject& data, MOAILuaState& state ) {

	// push class table
	lua_newtable ( state );
	this->RegisterLuaClass ( state );
	data.MOAILuaObject::RegisterLuaClass ( state );
	data.RegisterLuaClass ( state );
	
	// init the extend method
	state.PushPtrUserData ( &data ); // copy of userdata
	lua_pushvalue ( state, -2 ); // copy of class table
	lua_pushcclosure ( state, _extendSingleton, 2 );
	lua_setfield ( state, -2, "extend" );
	
	// ref class table
	this->mClassTable.SetRef ( state, -1 );
	
	lua_pushvalue ( state, -1 );
	lua_setfield ( state, -2, "__index" );

	lua_pushvalue ( state, -1 );
	lua_setfield ( state, -2, "__newindex" );

	lua_setglobal ( state, data.TypeName ());
}

//----------------------------------------------------------------//
bool MOAILuaClass::IsSingleton () {

	return ( this->mIsSingleton );
}

//----------------------------------------------------------------//
void MOAILuaClass::PushInterfaceTable ( MOAILuaState& state ) {

	state.Push ( this->mInterfaceTable );
}

//----------------------------------------------------------------//
void MOAILuaClass::PushRefTable ( MOAILuaState& state ) {

	if ( !this->mRefTable ) {
		lua_newtable ( state );
		this->mRefTable.SetRef ( state, -1 ); // strong ref to member table won't be garbage collected
	}
	else {
		this->mRefTable.PushRef ( state );
	}
}

//----------------------------------------------------------------//
MOAILuaClass::MOAILuaClass () :
	mIsSingleton ( false ) {
}

//----------------------------------------------------------------//
MOAILuaClass::~MOAILuaClass () {
}

