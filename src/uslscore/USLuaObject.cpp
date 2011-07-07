// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USLog.h>
#include <uslscore/USLuaObject.h>
#include <uslscore/USLuaRef.h>
#include <uslscore/USLuaState.h>
#include <uslscore/USLuaRuntime.h>
#include <uslscore/USLuaSerializer.h>
#include <uslscore/USLuaState-impl.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
int USLuaObject::_gc ( lua_State* L ) {

	USLuaState state ( L );
	
	USLuaObject* data = ( USLuaObject* )state.GetPtrUserData ( 1 );

	bool cleanup = data->mUserdata.IsWeak ();
	data->mUserdata.Clear ();
	
	if ( cleanup ) {
		delete data;
	}
	return 0;
}

//----------------------------------------------------------------//
int USLuaObject::_getClass ( lua_State* L ) {

	USLuaState state ( L );
	USLuaObject* object = ( USLuaObject* )state.GetPtrUserData ( 1 );

	if ( object ) {
		object->PushLuaClassTable ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
int USLuaObject::_getClassName ( lua_State* L ) {

	USLuaState state ( L );
	USLuaObject* object = ( USLuaObject* )state.GetPtrUserData ( 1 );

	if ( object ) {
		lua_pushstring ( L, object->TypeName ());
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: restore lua dump methods
//int USLuaObject::_tostring ( lua_State* L ) {
//
//	USLuaState state ( L );
//	if ( !state.CheckParams ( 1, "U" ) ) return 0;
//
//	USLuaObject* data = ( USLuaObject* )state.GetPtrUserData ( 1 );
//
//	lua_pushstring( state, data->ToStringWithType ().c_str () );
//	
//	return 1;
//}

//================================================================//
// USLuaObject
//================================================================//

//----------------------------------------------------------------//
void USLuaObject::DebugDump () {

	// TODO: fix for 64 bit
	USLog::Print ( "0x%08X <%s> %s", ( uint )(( u32 )this ), this->TypeName (), this->ToString ().c_str ());
}

//----------------------------------------------------------------//
STLString USLuaObject::ToString () {

	return STLString ();
}

//----------------------------------------------------------------//
STLString USLuaObject::ToStringWithType () {

	STLString members = this->ToString();

	STLString repr;
	repr.write ( "(%s) %s", this->TypeName (), members.c_str() );

	return repr;
}

//----------------------------------------------------------------//
USLuaClass* USLuaObject::GetLuaClass () {

	// no implementation
	assert ( false );
	return 0;
}

//----------------------------------------------------------------//
USLuaStateHandle USLuaObject::GetSelf () {

	USLuaStateHandle state = USLuaRuntime::Get ().State ();
	this->PushLuaUserdata ( state );
	return state;
}

//----------------------------------------------------------------//
bool USLuaObject::IsBound () {

	return ( this->mUserdata != 0 );
}

//----------------------------------------------------------------//
void USLuaObject::LuaUnbind ( USLuaState& state ) {
	
	if ( this->mUserdata ) {
		
		this->mUserdata.PushRef ( state );
		assert ( lua_isuserdata ( state, -1 ));
		
		void* userdata = lua_touserdata ( state, -1 );
		memset ( userdata, 0, sizeof ( void* ));
		
		lua_pushnil ( state );
		lua_setmetatable ( state, -2 );

		this->mUserdata.Clear ();
	}
}

//----------------------------------------------------------------//
void USLuaObject::OnRelease ( u32 refCount ) {

	if ( refCount == 0 ) {
	
		if ( this->mUserdata ) {
			assert ( !this->mUserdata.IsWeak ());
			this->mUserdata.MakeWeak ();
		}
		else {
			delete this;
		}
	}
}

//----------------------------------------------------------------//
void USLuaObject::OnRetain ( u32 refCount ) {
	UNUSED ( refCount );

	this->mUserdata.MakeStrong ();
}

//----------------------------------------------------------------//
void USLuaObject::PushLuaClassTable ( USLuaState& state ) {

	USLuaClass* luaType = this->GetLuaClass ();
	luaType->mClassTable.PushRef ( state );
}

//----------------------------------------------------------------//
void USLuaObject::PushLuaUserdata ( USLuaState& state ) {

	// create an instance table if none exists
	if ( this->mInstanceTable.IsNil ()) {
		
		USLuaClass* type = this->GetLuaClass ();
		assert ( type );
		
		// set the instance table
		lua_newtable ( state );
		type->InitLuaInstanceTable ( this, state, -1 );
		
		this->mInstanceTable = state.GetStrongRef ( -1 );
		state.Pop ( 1 );
	}

	// create the handle userdata for reference counting
	if ( !this->mUserdata.PushRef ( state )) {
		
		// pop the 'nil' pushed by PushRef
		state.Pop ( 1 );
		
		// create and initialize a new userdata
		state.PushPtrUserData ( this );

		// set the instance table
		this->mInstanceTable.PushRef ( state );

		// attach it to the handle
		lua_setmetatable ( state, -2 );
		
		// and take a ref back to the handle
		bool weak = ( this->GetRefCount () == 0 );
		this->mUserdata.SetRef ( state, -1, weak );
	}
}

//----------------------------------------------------------------//
void USLuaObject::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void USLuaObject::RegisterLuaFuncs ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void USLuaObject::SerializeIn ( USLuaState& state, USLuaSerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void USLuaObject::SerializeOut ( USLuaState& state, USLuaSerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void USLuaObject::SetLuaInstanceTable ( USLuaState& state, int idx ) {

	assert ( !this->mInstanceTable );

	if ( state.IsType ( idx, LUA_TTABLE )) {

		idx = state.AbsIndex ( idx );

		USLuaClass* type = this->GetLuaClass ();
		assert ( type );
		
		// set the instance table
		type->InitLuaInstanceTable ( this, state, idx );
		
		this->mInstanceTable = state.GetStrongRef ( idx );
	}
}

//----------------------------------------------------------------//
USLuaObject::USLuaObject () {
	RTTI_SINGLE ( RTTIBase )
}

//----------------------------------------------------------------//
USLuaObject::~USLuaObject () {

	// TODO: keep the tombstone idiom?
	if ( USLuaRuntime::IsValid ()) {
		
		if ( this->mUserdata ) {

			USLuaStateHandle state = USLuaRuntime::Get ().State ();
			this->LuaUnbind ( state );
		}
	}
}

//================================================================//
// USLuaClass
//================================================================//

//----------------------------------------------------------------//
USLuaObject* USLuaClass::GetSingleton () {
	return 0;
}

//----------------------------------------------------------------//
void USLuaClass::InitLuaFactoryClass ( USLuaObject& data, USLuaState& state ) {

	int top = lua_gettop ( state );

	lua_newtable ( state );
	
	lua_pushcfunction ( state, USLuaObject::_getClass );
	lua_setfield ( state, -2, "getClass" );
	
	lua_pushcfunction ( state, USLuaObject::_getClassName );
	lua_setfield ( state, -2, "getClassName" );

	data.RegisterLuaFuncs ( state );

	lua_pushvalue ( state, -1 );
	lua_setfield ( state, -2, "__index" );

	lua_pushnil ( state );
	lua_setfield ( state, -2, "__newindex" );

	//lua_pushcfunction ( state, _tostring );
	//lua_setfield ( state, -2, "__tostring" );

	this->mMemberTable = state.GetStrongRef ( -1 );
	
	lua_settop ( state, top );

	lua_newtable ( state );
	this->RegisterLuaClass ( state );
	data.RegisterLuaClass ( state );
	this->mClassTable = state.GetStrongRef ( -1 );

	lua_setglobal ( state, data.TypeName ());

	lua_settop ( state, top );
}

//----------------------------------------------------------------//
void USLuaClass::InitLuaInstanceTable ( USLuaObject* data, USLuaState& state, int idx ) {

	UNUSED ( data );
	
	idx = state.AbsIndex ( idx );
	
	lua_pushvalue ( state, idx );
	lua_setfield ( state, idx, "__index" );
	
	lua_pushvalue ( state, idx );
	lua_setfield ( state, idx, "__newindex" );

	lua_pushcfunction ( state, USLuaObject::_gc );
	lua_setfield ( state, idx, "__gc" );

	state.Push ( this->mMemberTable );
	lua_setmetatable ( state, idx );
}

//----------------------------------------------------------------//
void USLuaClass::InitLuaSingletonClass ( USLuaObject& data, USLuaState& state ) {

	int top = lua_gettop ( state );

	state.PushPtrUserData ( &data );

	lua_newtable ( state );
	this->RegisterLuaClass ( state );
	data.RegisterLuaClass ( state );
	this->mClassTable = state.GetStrongRef ( -1 );
	
	lua_pushvalue ( state, -1 );
	lua_setfield ( state, -2, "__index" );

	lua_pushnil ( state );
	lua_setfield ( state, -2, "__newindex" );

	lua_setmetatable ( state, -2 );

	lua_setglobal ( state, data.TypeName ());

	lua_settop ( state, top );
}

//----------------------------------------------------------------//
bool USLuaClass::IsSingleton () {

	return ( this->GetSingleton () != 0 );
}

//----------------------------------------------------------------//
USLuaClass::USLuaClass () {
}

//----------------------------------------------------------------//
USLuaClass::~USLuaClass () {
}

