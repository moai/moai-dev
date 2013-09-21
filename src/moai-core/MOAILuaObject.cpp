// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIDeserializer.h>
#include <moai-core/MOAILua.h>
#include <moai-core/MOAISerializer.h>

#define FINALIZE_FUNC_NAME "finalize"

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
int MOAILuaObject::_gc ( lua_State* L ) {

	MOAILuaState state ( L );
	MOAILuaObject* self = ( MOAILuaObject* )state.GetPtrUserData ( 1 );
	self->mCollected = true;
	
	if ( MOAILuaRuntime::IsValid ()) {
	
		if ( self->mFinalizer ) {
			self->mFinalizer.PushRef ( state );
			if ( state.IsType ( -1, LUA_TFUNCTION )) {
				state.DebugCall ( 0, 0 );
			}
			else if ( state.IsType ( -1, LUA_TSTRING )) {
				printf ( "%s\n", state.GetValue < cc8* >( -1, "" ));
			}
			else {
				state.Pop ( 1 );
			}
			self->mFinalizer.Clear ();
		}
	
		if ( MOAILuaRuntime::Get ().mReportGC ) {
			printf ( "GC %s <%p>\n", self->TypeName (), self );
		}
		MOAILuaRuntime::Get ().ClearObjectStackTrace ( self );
		MOAILuaRuntime::Get ().DeregisterObject ( *self );
	}
	
	if ( self->GetRefCount () == 0 ) {
		delete ( self );
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
int MOAILuaObject::_getRefTable ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILuaObject, "U" )

	self->PushRefTable ( state );
	return 1;
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
int MOAILuaObject::_setFinalizer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILuaObject, "U" )

	self->mFinalizer.SetRef ( state, 2 );
	return 0;
}


//----------------------------------------------------------------//
int MOAILuaObject::_setInterface ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILuaObject, "U" )

	self->SetInterfaceTable ( state, 2 );
	return 0;
}

//----------------------------------------------------------------//
int MOAILuaObject::_setMembers ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILuaObject, "U" )

	self->SetMemberTable ( state, 2 );
	return 0;
}

//----------------------------------------------------------------//
int MOAILuaObject::_pin ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILuaObject, "UU" )

	MOAILuaObject* object = state.GetLuaObject < MOAILuaObject >( -2, true );
	self->LuaRetain ( object );
	return 0;
}

//----------------------------------------------------------------//
int MOAILuaObject::_tostring ( lua_State* L ) {

	MOAILuaState state ( L );

	MOAILuaObject* self = ( MOAILuaObject* )state.GetPtrUserData ( 1 );
	if ( self ) {
		
		STLString str;
		
		lua_getfield ( state, 1, "getClassName" );
		if ( state.IsType ( -1, LUA_TFUNCTION )) {
		
			lua_pushvalue ( state, 1 );
			state.DebugCall ( 1, 1 );
			cc8* classname = state.GetValue < cc8* >( -1, "" );
			str.write ( "%p <%s>", self, classname );
			state.Push ( str );
			return 1;
		}
		
		str.write ( "%p <%s>", self, self->TypeName ());
		state.Push ( str );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAILuaObject::_unpin ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILuaObject, "UU" )

	MOAILuaObject* object = state.GetLuaObject < MOAILuaObject >( -2, true );
	self->LuaRelease ( object );
	return 0;
}

//================================================================//
// MOAILuaObject
//================================================================//

//----------------------------------------------------------------//
// userdata -> memberTable -> refTable -> interfaceTable
// userdata is the object
// memberTable is for ad hoc members added by the scripter
// refTable is for ad hoc mambers added by the engine
// interfaceTable is the 'class' table - methods and constants
// __index looks in membertTable then *skips* refTable to fall back on interfaceTable
void MOAILuaObject::BindToLua ( MOAILuaState& state ) {

	assert ( !this->mUserdata );
	
	if ( MOAILuaRuntime::IsValid ()) {
		MOAILuaRuntime::Get ().SetObjectStackTrace ( this );
		MOAILuaRuntime::Get ().RegisterObject ( *this );
	}
	
	MOAILuaClass* type = this->GetLuaClass ();
	assert ( type );
	
	assert ( !type->IsSingleton ());
	
	state.PushPtrUserData ( this ); // userdata
	
	lua_newtable ( state ); // ref table
	lua_newtable ( state ); // member table
	type->PushInterfaceTable ( state ); // interface table
	
	this->MakeLuaBinding ( state );
	
	// and take a weak ref back to the userdata	
	this->mUserdata.SetRef ( state, -1 );
	assert ( !lua_isnil ( state, -1 ));
}

//----------------------------------------------------------------//
MOAILuaClass* MOAILuaObject::GetLuaClass () {

	// no implementation
	assert ( false );
	return 0;
}

//----------------------------------------------------------------//
//cc8* MOAILuaObject::GetLuaClassName () {
//
//	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
//	cc8* classname = this->TypeName ();
//	
//	if ( this->mMemberTable ) {
//		state.Push ( this );
//		lua_getfield ( state, -1, "getClassName" );
//		
//		if ( state.IsType ( -1, LUA_TFUNCTION )) {
//			lua_pushvalue ( state, -2 );
//			state.DebugCall ( 1, 1 );
//			classname = state.GetValue < cc8* >( -1, "" );
//		}
//	}
//	return classname;
//}

//----------------------------------------------------------------//
MOAIScopedLuaState MOAILuaObject::GetSelf () {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	this->PushLuaUserdata ( state );
	return state;
}

//----------------------------------------------------------------//
void MOAILuaObject::GetRef ( MOAILuaRef& ref ) {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	this->PushLuaUserdata ( state );
	ref.SetRef ( state, -1 );
}

//----------------------------------------------------------------//
bool MOAILuaObject::IsBound () {

	return ( bool )this->mUserdata;
}

//----------------------------------------------------------------//
bool MOAILuaObject::IsSingleton () {

	MOAILuaClass* luaClass = this->GetLuaClass ();
	return luaClass->IsSingleton ();
}

//----------------------------------------------------------------//
void MOAILuaObject::LuaRelease ( MOAILuaObject* object ) {

	if ( !object ) return;
	
	if (( !this->mCollected ) && MOAILuaRuntime::IsValid ()) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		if ( this->PushRefTable ( state )) {
			if ( object->PushLuaUserdata ( state )) {
			
				lua_pushvalue ( state, -1 ); // copy the userdata
				lua_gettable ( state, -3 ); // get the count (or nil)
				u32 count = state.GetValue < u32 >( -1, 0 ); // get the count (or 0)
				lua_pop ( state, 1 ); // pop the old count
				
				if ( count == 0 ) return; // nothing to do
				
				if ( count > 1 ) {
					lua_pushnumber ( state, count - 1 ); // push the new count
				}
				else {
					lua_pushnil ( state );
				}
				lua_settable ( state, -3 ); // save it in the table
			}
		}
	}
	
	object->Release ();
}

//----------------------------------------------------------------//
void MOAILuaObject::LuaRetain ( MOAILuaObject* object ) {

	if ( !object ) return;
	object->Retain ();

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushRefTable ( state )) {
		if ( object->PushLuaUserdata ( state )) {
		
			lua_pushvalue ( state, -1 ); // copy the userdata
			lua_gettable ( state, -3 ); // get the count (or nil)
			u32 count = state.GetValue < u32 >( -1, 0 ); // get the count (or 0)
			lua_pop ( state, 1 ); // pop the old count
			lua_pushnumber ( state, count + 1 ); // push the new count
			lua_settable ( state, -3 ); // save it in the table
		}
	}
}

//----------------------------------------------------------------//
MOAILuaObject::MOAILuaObject ():
	mCollected ( false ) {
	RTTI_SINGLE ( RTTIBase )
}

//----------------------------------------------------------------//
MOAILuaObject::~MOAILuaObject () {

	if ( MOAILuaRuntime::IsValid ()) {
		
		if ( this->mUserdata ) {
			MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
			this->mUserdata.PushRef ( state );
			lua_pushnil ( state );
			lua_setmetatable ( state, -2 );
		}
	}
}

//----------------------------------------------------------------//
void MOAILuaObject::MakeLuaBinding ( MOAILuaState& state ) {

	// stack:
	// -1: interface table
	// -2: member table
	// -3: ref table
	// -4: userdata

	int top = state.GetTop ();
	int memberTable = top - 1;
	int refTable = top - 2;

	// ref table gets gc and tostring
	lua_pushcfunction ( state, MOAILuaObject::_gc );
	lua_setfield ( state, refTable, "__gc" );
	
	lua_pushcfunction ( state, MOAILuaObject::_tostring );
	lua_setfield ( state, refTable, "__tostring" );
	
	// member table is __index and __newindex for ref table
	lua_pushvalue ( state, memberTable );
	lua_setfield ( state, refTable, "__index" );
	
	lua_pushvalue ( state, memberTable );
	lua_setfield ( state, refTable, "__newindex" );
	
	// build the metatable stack
	lua_setmetatable ( state, -2 ); // interface is meta of member
	lua_setmetatable ( state, -2 ); // member is meta of ref
	lua_setmetatable ( state, -2 ); // ref is meta of userdata
}

//----------------------------------------------------------------//
void MOAILuaObject::OnRelease ( u32 refCount ) {

	// The engine is done with this object, so it's OK to delete
	// it if there is no connection to the Lua runtime. If there
	// is, then refcount can remain 0 and the object will be
	// collected by the Lua GC.

	if (( this->mCollected ) && ( refCount == 0 )) {
		// no Lua binding and no references, so
		// go ahead and kill this turkey
		delete this;
	}
}

//----------------------------------------------------------------//
void MOAILuaObject::PushLuaClassTable ( MOAILuaState& state ) {

	MOAILuaClass* luaType = this->GetLuaClass ();
	luaType->mClassTable.PushRef ( state );
}

//----------------------------------------------------------------//
bool MOAILuaObject::PushLuaUserdata ( MOAILuaState& state ) {

	if ( !this->mUserdata ) {
		this->BindToLua ( state );
		return true;
	}
	return this->mUserdata.PushRef ( state );
}

//----------------------------------------------------------------//
bool MOAILuaObject::PushMemberTable ( MOAILuaState& state ) {

	int top = state.GetTop ();

	if ( this->PushLuaUserdata ( state )) {
		if ( lua_getmetatable ( state, -1 )) {
			lua_replace ( state, -2 );
			if ( lua_getmetatable ( state, -1 )) {
				lua_replace ( state, -2 );
				return true;
			}
		}
	}
	state.SetTop ( top );
	lua_pushnil ( state );
	return false;
}

//----------------------------------------------------------------//
bool MOAILuaObject::PushRefTable ( MOAILuaState& state ) {

	MOAILuaClass* luaClass = this->GetLuaClass ();
	if ( !luaClass ) {
		lua_pushnil ( state );
		return false;
	}
		
	if ( luaClass->IsSingleton ()) {
		luaClass->PushRefTable ( state );
		return true;
	}

	if ( this->PushLuaUserdata ( state )) {
		int result = lua_getmetatable ( state, -1 );
		if ( result ) {
			lua_replace ( state, -2 );
			return true;
		}
	}
	return false;
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
		{ "getRefTable",			_getRefTable },
		{ "pin",					_pin },
		{ "setFinalizer",			_setFinalizer },
		{ "setInterface",			_setInterface },
		{ "setMembers",				_setMembers },
		{ "unpin",					_unpin },
		
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
void MOAILuaObject::SetInterfaceTable ( MOAILuaState& state, int idx ) {

	idx = state.AbsIndex ( idx );

	this->PushMemberTable ( state );
	
	// set the interface table as the member table's metatable
	lua_pushvalue ( state, idx );
	lua_setmetatable ( state, -2 );
	
	lua_pop ( state, 1 );
}

//----------------------------------------------------------------//
void MOAILuaObject::SetMemberTable ( MOAILuaState& state, int idx ) {
	UNUSED ( state );
	UNUSED ( idx );

	// TODO: what if object is a singleton?
	assert ( !this->GetLuaClass ()->IsSingleton ()); // TODO: should actually set the member table, not just crash

	idx = state.AbsIndex ( idx );
	
	this->PushLuaUserdata ( state ); // userdata
	lua_getmetatable ( state, - 1 ); // ref table
	lua_getmetatable ( state, - 1 ); // member table
	lua_getmetatable ( state, - 1 ); // interface table

	lua_pushvalue ( state, idx );
	lua_replace ( state, -3 );
	
	this->MakeLuaBinding ( state );
	state.Pop ( 1 );
}

//----------------------------------------------------------------//
bool MOAILuaObject::WasCollected () {
	return this->mCollected;
}
