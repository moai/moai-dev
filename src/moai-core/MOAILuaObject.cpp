// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIDeserializer.h>
#include <moai-core/MOAILua.h>
#include <moai-core/MOAISerializer.h>
#include <moai-core/MOAILuaState-impl.h>

#define FINALIZE_FUNC_NAME "finalize"
#define MOAI_TAG "moai"

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
int MOAILuaObject::_getMemberTable ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILuaObject, "U" )
	
	self->PushMemberTable ( state );
	return 1;
}

//----------------------------------------------------------------//
int MOAILuaObject::_getRefTable ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILuaObject, "U" )

	self->PushRefTable ( state );
	return 1;
}

//----------------------------------------------------------------//
int MOAILuaObject::_serializeIn ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILuaObject, "UT" )
	
	MOAIDeserializer dummy;
	self->SerializeIn ( state, dummy );
	return 1;
}

//----------------------------------------------------------------//
int MOAILuaObject::_serializeOut ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILuaObject, "UT" )
	
	MOAISerializer dummy;
	self->SerializeOut ( state, dummy );
	return 1;
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
		MOAILuaRuntime::Get ().RegisterObject ( state, *this );
	}
	
	MOAILuaClass* type = this->GetLuaClass ();
	assert ( type );
	
	// both singletons and instances may be bound to a userdata
	state.PushPtrUserData ( this ); // userdata
	
	// for now, singletons are just userdata with no add'l metatables
	// so only build the metatable stack if we're *not* a singleton
	if ( !type->IsSingleton ()) {
	
		// hang on to the userdata in case this object was created on the stack
		// shouldn't need to do this for singletons
		MOAILuaRuntime::Get ().CacheUserdata ( state, -1 );
	
		// instances get the 'full stack' of metatables
		lua_newtable ( state ); // ref table
		lua_newtable ( state ); // member table
		type->PushInterfaceTable ( state ); // interface table
		this->MakeLuaBinding ( state );
	}
	
	// and take a weak ref back to the userdata	
	this->mUserdata.SetRef ( state, -1 );
	assert ( !lua_isnil ( state, -1 ));
	
	// NOTE: we have to do this *after* mUserdata has been initialized as LuaRetain calls PushLuaUserdata
	// which in turn calls BindToLua if there is no mUserdata...
	if ( type->IsSingleton ()) {
		this->LuaRetain ( this ); // create a circular reference to 'pin' the userdata
		
		// in the case of a singleton, the ref table is kept in the class record, which is a strong
		// ref (as opposed to the metatable version attached to the weak userdata for regular instance
		// classes).
	}
}

//----------------------------------------------------------------//
MOAILuaClass* MOAILuaObject::GetLuaClass () {

	// no implementation
	assert ( false );
	return 0;
}

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
int MOAILuaObject::InjectAndCall ( lua_CFunction func, MOAILuaObject* self, lua_State* L ) {

	MOAILuaState state ( L );
	self->PushLuaUserdata ( state );
	lua_insert ( L, 1 );
	return func ( L );
}

//----------------------------------------------------------------//
bool MOAILuaObject::IsBound () {

	return ( bool )this->mUserdata;
}

//----------------------------------------------------------------//
bool MOAILuaObject::IsMoaiUserdata ( MOAILuaState& state, int idx ) {

	bool result = false;
	if ( state.IsType ( idx, LUA_TUSERDATA )) {
		if ( lua_getmetatable ( state, idx )) {
			result = state.HasField ( -1, MOAI_TAG );
			state.Pop ( 1 );
		}
	}
	return result;
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
				
				if ( count == 0 ) return; // do nothing
				
				if ( count > 1 ) {
					lua_pushnumber ( state, count - 1 ); // push the new count
				}
				else {
					lua_pushnil ( state );
				}
				
				// this should make the object eligible for garbage collection
				lua_settable ( state, -3 ); // save it in the table
			}
		}
	}
	
	// this will take the ref count to zero, but if the object hasn't been collected it *won't* get deleted
	// thanks to the override of MOAIObject OnRelease ()
	object->Release ();
}

//----------------------------------------------------------------//
void MOAILuaObject::LuaRetain ( MOAILuaObject* object ) {

	if ( !object ) return;
	object->Retain (); // strong ref

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushRefTable ( state )) {
		if ( object->PushLuaUserdata ( state )) {
		
			// look up the old count using the object as a key
			
			lua_pushvalue ( state, -1 ); // copy the userdata
			lua_gettable ( state, -3 ); // get the count (or nil)
			u32 count = state.GetValue < u32 >( -1, 0 ); // get the count (or 0)
			lua_pop ( state, 1 ); // pop the old count
			
			// store the new count in the ref table
			
			lua_pushnumber ( state, count + 1 ); // push the new count
			lua_settable ( state, -3 ); // save it in the table
		}
	}
}

//----------------------------------------------------------------//
MOAILuaObject::MOAILuaObject ():
	mCollected ( false ) {
	RTTI_SINGLE ( RTTIBase )
	
	if ( MOAILuaRuntime::IsValid ()) {
		MOAILuaRuntime::Get ().RegisterObject ( *this );
	}
}

//----------------------------------------------------------------//
MOAILuaObject::~MOAILuaObject () {

	if ( MOAILuaRuntime::IsValid ()) {
		
		MOAILuaRuntime::Get ().DeregisterObject ( *this );
		
		// TODO: change from both patrick's fork and the community branch; double check
		if (( !this->mCollected ) && this->mUserdata ) {
			MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
			
			// clear out the gc
			this->mUserdata.PushRef ( state );
			
			u32 top = state.GetTop ();
			bool isUserdata = state.IsType ( top, LUA_TUSERDATA );
			
			MOAILuaRuntime::Get ().PurgeUserdata ( state, -1 );
			
			top = state.GetTop ();
			isUserdata = state.IsType ( top, LUA_TUSERDATA );
			
			if ( lua_getmetatable ( state, -1 )) {
				lua_pushnil ( state );
				lua_setfield ( state, -2, "__gc" );
				state.Pop ( 1 );
			}
			
			top = state.GetTop ();
			isUserdata = state.IsType ( top, LUA_TUSERDATA );
			
			// and the ref table
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
	
	// ref table gets 'moai' tag set to true
	lua_pushboolean ( state, 1 );
	lua_setfield ( state, refTable, MOAI_TAG );
	
	
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

	if ( this->mCollected && ( refCount == 0 )) {
		// no Lua binding and no references, so
		// go ahead and kill this turkey
		delete this;
	}
}

//----------------------------------------------------------------//
void MOAILuaObject::PrintTracking () {

	MOAILuaRuntime::Get ().PrintTracking ( *this );
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

	MOAILuaClass* luaType = this->GetLuaClass ();
	if ( luaType->IsSingleton ()) {
		this->PushLuaClassTable ( state );
		return true;
	}

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
		{ "getMemberTable",			_getMemberTable },
		{ "getRefTable",			_getRefTable },
		{ "pin",					_pin },
		{ "serializeIn",			_serializeIn },
		{ "serializeOut",			_serializeOut },
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
