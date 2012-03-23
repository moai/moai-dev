// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILuaObject.h>
#include <moaicore/MOAILuaRef.h>
#include <moaicore/MOAILuaState.h>
#include <moaicore/MOAILuaRuntime.h>
#include <moaicore/MOAISerializer.h>
#include <moaicore/MOAILuaState-impl.h>

//================================================================//
// MOAILuaLocal
//================================================================//
	
//----------------------------------------------------------------//
MOAILuaLocal::MOAILuaLocal () :
	mRef ( LUA_NOREF ) {
}

//----------------------------------------------------------------//
MOAILuaLocal::~MOAILuaLocal () {
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
int MOAILuaObject::_gc ( lua_State* L ) {

	MOAILuaState state ( L );
	
	MOAILuaObject* data = ( MOAILuaObject* )state.GetPtrUserData ( 1 );

	bool cleanup = ( data->GetRefCount () == 0 ); // ready to cleanup if no references

	// in any event, let's get rid of the userdata and lua refs we know about
	data->mUserdata.Clear ();
	data->mMemberTable.Clear ();
	
	// check to see if gc is being invoked during finalization
	if ( MOAILuaRuntime::IsValid ()) {
		MOAILuaRuntime::Get ().ClearObjectStackTrace ( data );
	}

	// delete if no references
	if ( cleanup ) {
		delete data;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAILuaObject::_getClass ( lua_State* L ) {

	MOAILuaState state ( L );
	MOAILuaObject* object = ( MOAILuaObject* )state.GetPtrUserData ( 1 );

	if ( object ) {
		object->PushLuaClassTable ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAILuaObject::_getClassName ( lua_State* L ) {

	MOAILuaState state ( L );
	MOAILuaObject* object = ( MOAILuaObject* )state.GetPtrUserData ( 1 );

	if ( object ) {
		lua_pushstring ( L, object->TypeName ());
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAILuaObject::_setInterface ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILuaObject, "U" )

	self->SetInterfaceTable ( state, 2 );

	return 0;
}

//----------------------------------------------------------------//
int MOAILuaObject::_tombstone ( lua_State* L ) {

	MOAILuaState state ( L );
	
	USLog::Print ( "----------------------------------------------------------------\n" );
	USLog::Print ( "ERROR: Attempt to access missing object instance.\n" );
	state.PrintStackTrace ( USLog::CONSOLE, 0 );
	USLog::Print ( "\n" );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAILuaObject::_tostring ( lua_State* L ) {

	MOAILuaState state ( L );

	MOAILuaObject* data = ( MOAILuaObject* )state.GetPtrUserData ( 1 );
	if ( data ) {
		
		STLString str;
		
		lua_getfield ( state, 1, "getClassName" );
		if ( state.IsType ( -1, LUA_TFUNCTION )) {
		
			lua_pushvalue ( state, 1 );
			state.DebugCall ( 1, 1 );
			cc8* classname = state.GetValue < cc8* >( -1, "" );
			str.write ( "%p <%s>", data, classname );
			state.Push ( str );
			return 1;
		}
		
		str.write ( "%p <%s>", data, data->TypeName ());
		state.Push ( str );
		return 1;
	}
	return 0;
}

//================================================================//
// MOAILuaObject
//================================================================//

//----------------------------------------------------------------//
void MOAILuaObject::BindToLua ( MOAILuaState& state ) {

	assert ( !this->mUserdata );
	
	MOAILuaClass* type = this->GetLuaClass ();
	assert ( type );
	
	// create and initialize a new userdata
	state.PushPtrUserData ( this );
	
	// create and initialize the member table
	lua_newtable ( state );
	
	lua_pushvalue ( state, -1 );
	lua_setfield ( state, -2, "__index" );
	
	lua_pushvalue ( state, -1 );
	lua_setfield ( state, -2, "__newindex" );
	
	lua_pushcfunction ( state, MOAILuaObject::_gc );
	lua_setfield ( state, -2, "__gc" );
	
	lua_pushcfunction ( state, MOAILuaObject::_tostring );
	lua_setfield ( state, -2, "__tostring" );
	
	// attach the member table to the userdata
	lua_pushvalue ( state, -1 );
	lua_setmetatable ( state, -3 );
	
	// grab a ref to the member table
	this->mMemberTable = state.GetWeakRef ( -1 );
	
	// stack:
	// -1: member table
	// -2: userdata
	
	// create an empty ref table and attach it to the member table
	lua_newtable ( state );
	
	lua_pushvalue ( state, -1 );
	lua_setmetatable ( state, -3 );
	
	// stack:
	// -1: ref table
	// -2: member table
	// -3: userdata
	
	// push the interface table and attach it to the ref table
	type->PushInterfaceTable ( state );
	
	lua_pushvalue ( state, -1 );
	lua_setmetatable ( state, -3 );
	
	// stack:
	// -1: interface table
	// -2: ref table
	// -3: member table
	// -4: userdata
	
	// use the interface table as the ref table's __index
	lua_setfield ( state, -2, "__index" );
	
	// stack:
	// -1: ref table
	// -2: member table
	// -3: userdata
	
	lua_pop ( state, 2 );
	
	// stack:
	// -1: userdata
	
	// and take a ref back to the userdata	
	if ( this->GetRefCount () == 0 ) {
		this->mUserdata.SetWeakRef ( state, -1 );
	}
	else {
		this->mUserdata.SetStrongRef ( state, -1 );
	}
	
	assert ( !lua_isnil ( state, -1 ));
}

//----------------------------------------------------------------//
void MOAILuaObject::ClearLocal ( MOAILuaLocal& ref ) {
	
	if ( MOAILuaRuntime::IsValid ()) {
		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		
		this->PushRefTable ( state );
		
		lua_pushnumber ( state, ref.mRef );
		lua_pushnil ( state );
		lua_settable ( state, -3 );
		lua_pop ( state, 1 );
	}
	ref.mRef = LUA_NOREF;
}

//----------------------------------------------------------------//
MOAILuaClass* MOAILuaObject::GetLuaClass () {

	// no implementation
	assert ( false );
	return 0;
}

//----------------------------------------------------------------//
cc8* MOAILuaObject::GetLuaClassName () {

	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	cc8* classname = this->TypeName ();
			
	state.Push ( this );
	lua_getfield ( state, -1, "getClassName" );
	
	if ( state.IsType ( -1, LUA_TFUNCTION )) {
		lua_pushvalue ( state, -2 );
		state.DebugCall ( 1, 1 );
		classname = state.GetValue < cc8* >( -1, "" );
	}
	return classname;
}

//----------------------------------------------------------------//
MOAILuaStateHandle MOAILuaObject::GetSelf () {

	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	this->PushLuaUserdata ( state );
	return state;
}

//----------------------------------------------------------------//
bool MOAILuaObject::IsBound () {

	return ( this->mUserdata != 0 );
}

//----------------------------------------------------------------//
bool MOAILuaObject::IsSingleton () {

	MOAILuaClass* luaClass = this->GetLuaClass ();
	return luaClass->IsSingleton ();
}

//----------------------------------------------------------------//
void MOAILuaObject::LockToRefCount () {

	this->mUserdata.MakeStrong ();
}

//----------------------------------------------------------------//
void MOAILuaObject::LuaRelease ( MOAILuaObject* object ) {

	if ( !object ) return;
	if ( !MOAILuaRuntime::IsValid ()) return;

	// get ref table
	MOAILuaStateHandle state = this->GetSelf ();
	state.Push ( this->mMemberTable );
	int result = lua_getmetatable ( state, -1 );
	assert ( result );
	
	// stack:
	// -1: ref table
	// -2: member table
	// -3: userdata
	
	lua_replace ( state, -2 );
	
	// stack:
	// -1: ref table
	// -2: userdata
	
	// push the object to retain
	state.Push ( object );

	// check to see if the object is already in the table
	lua_pushvalue ( state, -1 );
	lua_rawget ( state, -3 );
	
	// stack:
	// -1: object OR nil
	// -2: object
	// -3: ref table
	// -4: userdata
	
	if ( lua_type ( state, -1 ) != LUA_TNIL ) {
		lua_pop ( state, 2 );
		lua_pushnil ( state );
		lua_rawset ( state, -2 );
		object->Release ();
	}
}

//----------------------------------------------------------------//
void MOAILuaObject::LuaRetain ( MOAILuaObject* object ) {

	if ( !object ) return;

	// get ref table
	MOAILuaStateHandle state = this->GetSelf ();
	state.Push ( this->mMemberTable );
	int result = lua_getmetatable ( state, -1 );
	assert ( result );
	
	// stack:
	// -1: ref table
	// -2: member table
	// -3: userdata
	
	lua_replace ( state, -2 );
	
	// stack:
	// -1: ref table
	// -2: userdata
	
	// push the object to retain
	state.Push ( object );

	// check to see if the object is already in the table
	lua_pushvalue ( state, -1 );
	lua_rawget ( state, -3 );
	
	// stack:
	// -1: object OR nil
	// -2: object
	// -3: ref table
	// -4: userdata
	
	if ( lua_type ( state, -1 ) == LUA_TNIL ) {
		lua_pop ( state, 1 );
		lua_rawset ( state, -2 );
		object->Retain ();
	}
}

//----------------------------------------------------------------//
void MOAILuaObject::LuaUnbind () {
	
	if ( this->mUserdata && MOAILuaRuntime::IsValid ()) {
		
		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		
		this->mUserdata.PushRef ( state );
		
		void* userdata = lua_touserdata ( state, -1 );
		memset ( userdata, 0, sizeof ( void* ));
		
		lua_newtable ( state );
		
		lua_pushvalue ( state, -1 );
		lua_setmetatable ( state, -3 );
		
		lua_pushcfunction ( state, MOAILuaObject::_tombstone );
		lua_setfield ( state, -2, "__index" );
		
		lua_pushcfunction ( state, MOAILuaObject::_tombstone );
		lua_setfield ( state, -2, "__newindex" );
		
		lua_pushcfunction ( state, MOAILuaObject::_tombstone );
		lua_setfield ( state, -2, "__tostring" );
		
		lua_setmetatable ( state, -2 );

		lua_pop ( state, 1 );

		this->mUserdata.Clear ();
	}
}

//----------------------------------------------------------------//
void MOAILuaObject::OnRelease ( u32 refCount ) {

	if ( refCount == 0 ) {
	
		if ( this->mUserdata ) {
			this->mUserdata.MakeWeak ();
		}
		else {
			// no Lua binding and no references, so
			// go ahead and kill this turkey
			delete this;
		}
	}
}

//----------------------------------------------------------------//
void MOAILuaObject::OnRetain ( u32 refCount ) {
	UNUSED ( refCount );
}

//----------------------------------------------------------------//
void MOAILuaObject::PushLuaClassTable ( MOAILuaState& state ) {

	MOAILuaClass* luaType = this->GetLuaClass ();
	luaType->mClassTable.PushRef ( state );
}

//----------------------------------------------------------------//
void MOAILuaObject::PushLuaUserdata ( MOAILuaState& state ) {

	bool hasUserdata = !this->mUserdata.IsNil ();

	// create the handle userdata for reference counting
	if ( !this->mUserdata.PushRef ( state )) {
		
		// pop the 'nil' pushed by PushRef
		state.Pop ( 1 );
		
		// this is a nasty edge case where the userdata has been tagged for garbage
		// collection, but not actually collected. the result is that the ref hasn't
		// be cleared yet, but when we push it we get nil. this should only happen to
		// refs to userdata. it's tempting to try and clear out the ref here, but if
		// the ref is to a MOAILuaObject's userdata, the next step may be to recreate
		// the object... which means when it is garbage collected the wrong (new)
		// userdata will be cleaned up! so all we can do is force a full collection
		// step, set ourselves to nil and return failure.
		if ( hasUserdata ) {
			USLog::Print ( "Attempt to access MOAILuaObject userdata tagged for garbage collection; running a full cycle of GC prior to reallocation.\n" );
			MOAILuaRuntime::Get ().ForceGarbageCollection ();
		}
		
		// initialize and bind the userdata
		this->BindToLua ( state );
	}
	assert ( !lua_isnil ( state, -1 ));
}

//----------------------------------------------------------------//
bool MOAILuaObject::PushLocal ( MOAILuaState& state, MOAILuaLocal& ref ) {

	if ( ref ) {
		
		this->PushRefTable ( state );
		
		lua_rawgeti ( state, -1, ref.mRef );
		lua_replace ( state, -2 );
		return true;
	}
	lua_pushnil ( state );
	return false;
}

//----------------------------------------------------------------//
void MOAILuaObject::PushMemberTable ( MOAILuaState& state ) {

	assert ( this->mMemberTable );
	this->mMemberTable.PushRef ( state );
}

//----------------------------------------------------------------//
void MOAILuaObject::PushRefTable ( MOAILuaState& state ) {

	assert ( this->mMemberTable );
	this->mMemberTable.PushRef ( state );
	
	int result = lua_getmetatable ( state, -1 );
	assert ( result );
	
	lua_replace ( state, -2 );
}

//----------------------------------------------------------------//
void MOAILuaObject::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAILuaObject::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getClass",				_getClass },
		{ "getClassName",			_getClassName },
		{ "setInterface",			_setInterface },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAILuaObject::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAILuaObject::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAILuaObject::SetLocal ( MOAILuaState& state, int idx, MOAILuaLocal& ref ) {

	idx = state.AbsIndex ( idx );

	this->PushRefTable ( state );
	
	if ( ref ) {
		luaL_unref ( state, -1, ref.mRef );
		ref.mRef = LUA_NOREF;
	}
	
	state.CopyToTop ( idx );
	ref.mRef = luaL_ref ( state, -2 );
	
	lua_pop ( state, 1 );
}

//----------------------------------------------------------------//
void MOAILuaObject::SetInterfaceTable ( MOAILuaState& state, int idx ) {

	idx = state.AbsIndex ( idx );

	this->PushRefTable ( state );
	
	// set the interface table as the ref table's metatable
	lua_pushvalue ( state, idx );
	lua_setmetatable ( state, -2 );
	
	// use the interface table as the ref table's __index
	lua_pushvalue ( state, idx );
	lua_setfield ( state, -2, "__index" );
	
	lua_pop ( state, 1 );
}

//----------------------------------------------------------------//
void MOAILuaObject::SetMemberTable ( MOAILuaState& state, int idx ) {

	// TODO: what is object is a singleton?
	MOAILuaClass* luaClass = this->GetLuaClass ();
	assert ( !luaClass->IsSingleton ()); // TODO: should actually set the member table, not just crash

	idx = state.AbsIndex ( idx );

	assert ( this->mUserdata );
	assert ( this->mMemberTable );

	state.Push ( this->mUserdata ); // userdata
	state.Push ( this->mMemberTable ); // member table
	
	int result = lua_getmetatable ( state, -1 ); // ref table
	assert ( result );
	
	lua_pushvalue ( state, idx ); // new member table
	
	// stack:
	// -1: new member table
	// -2: ref table
	// -3: member table
	// -4: userdata
	
	lua_pushvalue ( state, idx );
	lua_setfield ( state, -2, "__index" );
	
	lua_pushvalue ( state, idx );
	lua_setfield ( state, -2, "__newindex" );
	
	lua_pushcfunction ( state, MOAILuaObject::_gc );
	lua_setfield ( state, -2, "__gc" );
	
	lua_pushcfunction ( state, MOAILuaObject::_tostring );
	lua_setfield ( state, -2, "__tostring" );
	
	lua_pop ( state, 1 );
	
	// stack:
	// -1: ref table
	// -2: member table
	// -3: userdata
	
	lua_pushvalue ( state, -3 ); // userdata
	lua_pushvalue ( state, idx ); // new member table
	lua_setmetatable ( state, -2 );
	lua_pop ( state, 1 );
	
	lua_pushvalue ( state, idx ); // new member table
	lua_pushvalue ( state, -2 ); // ref table
	lua_setmetatable ( state, -2 );
	lua_pop ( state, 1 );
	
	// stack:
	// -1: ref table
	// -2: member table
	// -3: userdata
	
	lua_pop ( state, 3 );
}

//----------------------------------------------------------------//
MOAILuaObject::MOAILuaObject () {
	RTTI_SINGLE ( RTTIBase )
	
	if ( MOAILuaRuntime::IsValid ()) {
		MOAILuaRuntime::Get ().RegisterObject ( *this );
	}
}

//----------------------------------------------------------------//
MOAILuaObject::~MOAILuaObject () {
	
	if ( MOAILuaRuntime::IsValid ()) {
		
		MOAILuaRuntime::Get ().ClearObjectStackTrace ( this );
		
		this->LuaUnbind ();
		
		MOAILuaRuntime::Get ().DeregisterObject ( *this );
	}
}

//================================================================//
// MOAILuaClass
//================================================================//

//----------------------------------------------------------------//
int MOAILuaClass::_extendFactory ( lua_State* L ) {

	MOAILuaState state ( L );

	// clone the class table
	state.CloneTable ( lua_upvalueindex ( 1 ));
	
	// add getClassName to class table
	lua_pushvalue ( L, 1 );
	lua_pushcclosure ( L, _getUpvalue, 1 );
	lua_setfield ( L, -2, "getClassName" );
	
	// clone the interface table
	state.CloneTable ( lua_upvalueindex ( 2 ));
	
	// add getClass to interface table
	lua_pushvalue ( L, -2 );
	lua_pushcclosure ( L, _getUpvalue, 1 );
	lua_setfield ( L, -2, "getClass" );
	
	// add getClassName to interface table
	lua_pushvalue ( L, 1 );
	lua_pushcclosure ( L, _getUpvalue, 1 );
	lua_setfield ( L, -2, "getClassName" );
	
	// call the interface extender
	if ( state.IsType ( 2, LUA_TFUNCTION )) {
		lua_pushvalue ( L, 2 );
		lua_pushvalue ( L, -2 );
		lua_pushvalue ( L, lua_upvalueindex ( 2 ));
		state.DebugCall ( 2, 0 );
	}
	
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
	
	// now copy the extended class and interface tables
	lua_pushvalue ( L, -2 );
	lua_pushvalue ( L, -2 );
	
	// push the 'extend' method with the extended class and interface tables as upvalues
	lua_pushcclosure ( L, _extendFactory, 2 );
	
	// set the extended 'extend' method into the class table
	lua_setfield ( L, -3, "extend" );
	
	// init the getInterfaceTable method
	lua_pushvalue ( L, -1 );
	lua_pushcclosure ( state, _getInterfaceTable, 1 );
	lua_setfield ( state, -3, "getInterfaceTable" );
	
	// done with the extended interface table
	lua_pop ( L, 1 );

	// call the class extender
	if ( state.IsType ( 3, LUA_TFUNCTION )) {
		lua_pushvalue ( L, 3 );
		lua_pushvalue ( L, -2 );
		lua_pushvalue ( L, lua_upvalueindex ( 1 ));
		state.DebugCall ( 2, 0 );
	}

	// and we're done
	cc8* classname = state.GetValue < cc8* >( 1, "" );
	lua_setglobal ( state, classname );
	return 0;
}

//----------------------------------------------------------------//
int MOAILuaClass::_extendSingleton ( lua_State* L ) {

	MOAILuaState state ( L );
	
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
	
	// call the class extender
	if ( state.IsType ( 2, LUA_TFUNCTION )) {
		lua_pushvalue ( L, 2 );
		lua_pushvalue ( L, -2 );
		lua_pushvalue ( L, lua_upvalueindex ( 2 ));
		state.DebugCall ( 2, 0 );
	}
	
	// copy the extended table
	lua_pushvalue ( L, -1 );
	
	// push the 'extend' method with the extended class table and factory method as upvalue
	lua_pushcclosure ( L, _extendSingleton, 1 );
	
	// set the extended 'extend' method...
	lua_setfield ( L, -2, "extend" );

	// set the table as a metatable on the userdata
	MOAILuaObject* luaData = ( MOAILuaObject* )state.GetPtrUserData ( lua_upvalueindex ( 1 ));
	state.PushPtrUserData ( luaData );
	lua_insert ( L, -2 );
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
	// 1: original 'new'
	// 2: interface table

	MOAILuaState state ( L );
	
	lua_pushvalue ( L, lua_upvalueindex ( 2 ));
	if ( state.IsType ( -1, LUA_TFUNCTION )) {
	
		// call the original new
		state.DebugCall ( 0, 1 );
		
		if ( state.IsType ( -1, LUA_TUSERDATA )) {
		
			// get the member table
			if ( lua_getmetatable ( state, -1 )) {
				
				// get the ref table
				if ( lua_getmetatable ( state, -1 )) {
				
					// get the interface table
					lua_pushvalue ( L, lua_upvalueindex ( 1 ));
					
					// set the interface table as the metatable
					lua_pushvalue ( state, -1 );
					lua_setmetatable ( L, -3 );
					
					// set the interface table as the __index metamethod
					lua_setfield ( state, -2, "__index" );
					
					// done with the ref table
					lua_pop ( L, 1 );
				}
				// done with the member table
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
	
	data.MOAILuaObject::RegisterLuaFuncs ( state );
	data.RegisterLuaFuncs ( state );

	this->mInterfaceTable = state.GetStrongRef ( -1 );
	
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
	this->mClassTable = state.GetStrongRef ( -1 );
	lua_setglobal ( state, data.TypeName ());

	// reset top
	lua_settop ( state, top );
}

//----------------------------------------------------------------//
void MOAILuaClass::InitLuaSingletonClass ( MOAILuaObject& data, MOAILuaState& state ) {

	int top = lua_gettop ( state );

	state.PushPtrUserData ( &data );

	// push class table
	lua_newtable ( state );
	this->RegisterLuaClass ( state );
	data.MOAILuaObject::RegisterLuaClass ( state );
	data.RegisterLuaClass ( state );
	
	// init the extend method
	lua_pushvalue ( state, -2 ); // copy of userdata
	lua_pushvalue ( state, -2 ); // copy of class table
	lua_pushcclosure ( state, _extendSingleton, 2 );
	lua_setfield ( state, -2, "extend" );
	
	// ref class table
	this->mClassTable = state.GetStrongRef ( -1 );
	
	lua_pushvalue ( state, -1 );
	lua_setfield ( state, -2, "__index" );

	lua_pushvalue ( state, -1 );
	lua_setfield ( state, -2, "__newindex" );

	lua_setmetatable ( state, -2 );

	lua_setglobal ( state, data.TypeName ());

	// set up the member and instance tables so we can use lua retain/release
	lua_newtable ( state );
	lua_newtable ( state );
	lua_setmetatable ( state, -2 );
	data.mMemberTable.SetStrongRef ( state, -1 );
	lua_pop ( state, 1 );

	lua_settop ( state, top );
}

//----------------------------------------------------------------//
bool MOAILuaClass::IsSingleton () {

	return ( this->GetSingleton () != 0 );
}

//----------------------------------------------------------------//
void MOAILuaClass::PushInterfaceTable ( MOAILuaState& state ) {

	state.Push ( this->mInterfaceTable );
}

//----------------------------------------------------------------//
MOAILuaClass::MOAILuaClass () {
}

//----------------------------------------------------------------//
MOAILuaClass::~MOAILuaClass () {
}

