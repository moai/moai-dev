// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USLuaData.h>
#include <uslscore/USLuaRef.h>
#include <uslscore/USLuaState.h>
#include <uslscore/USLuaRuntime.h>
#include <uslscore/USLuaSerializer.h>
#include <uslscore/USLuaState-impl.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
int USLuaData::_delete ( lua_State* L ) {

	USLuaState state ( L );

	USLuaData* data = ( USLuaData* )state.GetPtrUserData ( 1 );

	data->mSelf.Clear ();
	data->Release ();

	return 0;
}

//----------------------------------------------------------------//
// TODO: restore lua dump methods
//int USLuaData::_tostring ( lua_State* L ) {
//
//	USLuaState state ( L );
//	if ( !state.CheckParams ( 1, "U" ) ) return 0;
//
//	USLuaData* data = ( USLuaData* )state.GetPtrUserData ( 1 );
//
//	lua_pushstring( state, data->ToStringWithType ().c_str () );
//	
//	return 1;
//}

//================================================================//
// USLuaData
//================================================================//

//----------------------------------------------------------------//
void USLuaData::DebugDump () {

	// TODO: fix for 64 bit
	printf ( "0x%08X <%s> %s", ( uint )(( u32 )this ), this->TypeName (), this->ToString ().c_str ());
}

//----------------------------------------------------------------//
STLString USLuaData::ToString () {

	return STLString ();
}

//----------------------------------------------------------------//
STLString USLuaData::ToStringWithType () {

	STLString members = this->ToString();

	STLString repr;
	repr.write ( "(%s) %s", this->TypeName (), members.c_str() );

	return repr;
}

//----------------------------------------------------------------//
USLuaDataType* USLuaData::GetLuaDataType () {

	// no implementation
	assert ( false );
	return 0;
}

//----------------------------------------------------------------//
USLuaStateHandle USLuaData::GetSelf () {

	USLuaStateHandle state = USLuaRuntime::Get ().State ();
	this->PushLuaInstance ( state );
	return state;
}

//----------------------------------------------------------------//
bool USLuaData::IsBound () {

	return ( this->mSelf != 0 );
}

//----------------------------------------------------------------//
void USLuaData::LuaUnbind ( USLuaState& state ) {
	
	if ( this->mSelf ) {
		
		this->mSelf.PushRef ( state );
		
		// push tombstone
		lua_newtable ( state );
			
		lua_pushvalue ( state, -1 );
		lua_setfield ( state, -2, "__index" );
			
		lua_pushnil ( state );
		lua_setfield ( state, -2, "__newindex" );
	
		lua_setmetatable ( state, -2 );
		lua_pop ( state, 1 );

		this->mSelf.Clear ();
		this->mInstance.Clear ();
	}
}

//----------------------------------------------------------------//
void USLuaData::PushLuaClass ( USLuaState& state ) {

	USLuaDataType* luaType = this->GetLuaDataType ();
	luaType->mClassTable.PushRef ( state );
}

//----------------------------------------------------------------//
void USLuaData::PushLuaInstance ( USLuaState& state ) {

	// create an instance table if none exists
	if ( this->mInstance.IsNil ()) {
		
		USLuaDataType* type = this->GetLuaDataType ();
		assert ( type );
		
		// set the instance table
		lua_newtable ( state );
		type->InitLuaInstance ( this, state, -1 );
		
		this->mInstance = state.GetStrongRef ( -1 );
		state.Pop ( 1 );
	}

	// create the handle userdata for reference counting
	if ( this->mSelf.IsNil ()) {
		
		// create and initialize the userdata
		state.PushPtrUserData ( this );

		// set the instance table
		this->mInstance.PushRef ( state );

		// attach it to the handle
		lua_setmetatable ( state, -2 );
		
		// and take a ref back to the handle
		this->mSelf.SetRef ( state, -1, true );
		
		// and retain
		this->Retain ();
	}
	else {
		
		// re-use the handle
		this->mSelf.PushRef ( state );
	}
}

//----------------------------------------------------------------//
void USLuaData::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void USLuaData::RegisterLuaFuncs ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void USLuaData::SerializeIn ( USLuaState& state, USLuaSerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void USLuaData::SerializeOut ( USLuaState& state, USLuaSerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void USLuaData::SetLuaInstance ( USLuaState& state, int idx ) {

	assert ( !this->mInstance );

	if ( state.IsType ( idx, LUA_TTABLE )) {

		idx = state.AbsIndex ( idx );

		USLuaDataType* type = this->GetLuaDataType ();
		assert ( type );
		
		// set the instance table
		type->InitLuaInstance ( this, state, idx );
		
		this->mInstance = state.GetStrongRef ( idx );
	}
}

//----------------------------------------------------------------//
USLuaData::USLuaData () {
	RTTI_SINGLE ( RTTIBase )
}

//----------------------------------------------------------------//
USLuaData::~USLuaData () {

	// TODO:
	if (( !this->mSelf.IsNil ()) && USLuaRuntime::IsValid ()) {
		USLuaStateHandle state = USLuaRuntime::Get ().State ();
		this->LuaUnbind ( state );
	}
}

//================================================================//
// USLuaDataType
//================================================================//

//----------------------------------------------------------------//
USLuaData* USLuaDataType::GetSingleton () {
	return 0;
}

//----------------------------------------------------------------//
void USLuaDataType::InitLuaData ( USLuaData& data, USLuaState& state ) {

	int top = lua_gettop ( state );

	lua_newtable ( state );
	data.RegisterLuaFuncs ( state );

	lua_pushvalue ( state, -1 );
	lua_setfield ( state, -2, "__index" );

	lua_pushnil ( state );
	lua_setfield ( state, -2, "__newindex" );

	this->mInstanceTable = state.GetStrongRef ( -1 );
	
	lua_settop ( state, top );

	lua_newtable ( state );
	this->RegisterLuaClass ( state );
	data.RegisterLuaClass ( state );
	this->mClassTable = state.GetStrongRef ( -1 );

	lua_setglobal ( state, data.TypeName ());

	lua_settop ( state, top );
}

//----------------------------------------------------------------//
void USLuaDataType::InitLuaInstance ( USLuaData* data, USLuaState& state, int idx ) {

	UNUSED ( data );
	
	idx = state.AbsIndex ( idx );
	
	lua_pushvalue ( state, idx );
	lua_setfield ( state, idx, "__index" );
	
	lua_pushvalue ( state, idx );
	lua_setfield ( state, idx, "__newindex" );
	
	lua_pushcfunction ( state, USLuaData::_delete );
	lua_setfield ( state, idx, "__gc" );

	//lua_pushcfunction ( state, _tostring );
	//lua_setfield ( state, idx, "__tostring" );
	
	state.Push ( this->mInstanceTable );
	lua_setmetatable ( state, idx );
}

//----------------------------------------------------------------//
void USLuaDataType::InitLuaSingleton ( USLuaData& data, USLuaState& state ) {

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
bool USLuaDataType::IsSingleton () {

	return ( this->GetSingleton () != 0 );
}

//----------------------------------------------------------------//
USLuaDataType::USLuaDataType () {
}

//----------------------------------------------------------------//
USLuaDataType::~USLuaDataType () {
}

