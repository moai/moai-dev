// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIDeserializer.h>
#include <moai-core/MOAILua.h>
#include <moai-core/MOAIPool.h>
#include <moai-core/MOAISerializer.h>
#include <moai-core/MOAILuaState-impl.h>
#include <moai-core/strings.h>

#define FINALIZE_FUNC_NAME "finalize"
#define MOAI_TAG "moai"

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAILuaObject::_alertNewIsUnsupported ( lua_State* L ) {

	MOAILogF ( L, ZLLog::LOG_ERROR, MOAISTRING_NewIsUnsupported );
	return 0;
}

//----------------------------------------------------------------//
int MOAILuaObject::_gc ( lua_State* L ) {

	MOAILuaState state ( L );
	MOAILuaObject* self = ( MOAILuaObject* )state.GetPtrUserData ( 1 );
	
	MOAILuaRuntime& runtime = state.GetContext ()->Get < MOAILuaRuntime >();
	
	// edgecase: ignore _gc() called by previous Lua userdata
	self->mActiveUserdataCount--;
	if ( self->mActiveUserdataCount > 0 ) return 0;
	
	if ( runtime.IsValid ()) {
		
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
		
		if ( runtime.mReportGC ) {
			printf ( "GC %s <%p>\n", self->TypeName (), self );
		}
		
		self->mUserdata.Clear ();
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
	
	MOAILuaRuntime& runtime = this->Get < MOAILuaRuntime >();
	
	if ( runtime.IsValid ()) {
		runtime.RegisterObject ( state, *this );
	}
	
	MOAILuaClass* type = this->GetLuaClass ();
	assert ( type );
	
	// TODO: this exposes the class name to Lua, so no anonymous types.
	// TODO: if we want that will need to postpone making the class global until explicit registration.
	type->Register ( this );
	
	// both singletons and instances may be bound to a userdata
	state.PushPtrUserData ( this ); // userdata
	
	// hang on to the userdata in case this object was created on the stack
	// shouldn't need to do this for singletons
	runtime.CacheUserdata ( state, -1 );
	
	// instances get the 'full stack' of metatables
	lua_newtable ( state ); // ref table
	lua_newtable ( state ); // member table
	type->PushInterfaceTable ( state ); // interface table
	this->MakeLuaBinding ( state );
	
	// and take a weak ref back to the userdata
	this->mUserdata.SetRef ( state, -1 );
	assert ( this->mUserdata );
	this->mActiveUserdataCount++;
}

//----------------------------------------------------------------//
MOAILuaClass* MOAILuaObject::GetLuaClass () {

	// no implementation
	assert ( false );
	return 0;
}

//----------------------------------------------------------------//
MOAIScopedLuaState MOAILuaObject::GetSelf () {

	MOAIScopedLuaState state = this->Get < MOAILuaRuntime >().State ();
	this->PushLuaUserdata ( state );
	return state;
}

//----------------------------------------------------------------//
void MOAILuaObject::GetRef ( MOAILuaRef& ref ) {

	MOAIScopedLuaState state = this->Get < MOAILuaRuntime >().State ();
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
bool MOAILuaObject::IsInPool () {

	return ( this->mPoolType != NOT_IN_POOL );
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
void MOAILuaObject::LuaRelease () {

	this->LuaRelease ( this );
}

//----------------------------------------------------------------//
void MOAILuaObject::LuaRelease ( MOAILuaObject* object ) {

	if ( !object ) return;
	
	MOAILuaRuntime& runtime = this->Get < MOAILuaRuntime >();
	
	if (( this->IsBound ()) && runtime.IsValid ()) {
		MOAIScopedLuaState state = runtime.State ();
		if ( this->PushRefTable ( state )) {
			if ( object->PushLuaUserdata ( state )) {
			
				lua_pushvalue ( state, -1 ); // copy the userdata
				lua_gettable ( state, -3 ); // get the count (or nil)
				u32 count = state.GetValue < u32 >( -1, 0 ); // get the count (or 0)
				lua_pop ( state, 1 ); // pop the old count
				
				if ( count == 0 ) {
					lua_pop ( state, 2 ); // pop userdata && ref table
					return; // do nothing
				}
				
				if ( count > 1 ) {
					lua_pushnumber ( state, count - 1 ); // push the new count
				}
				else {
					lua_pushnil ( state );
				}
				
				// this should make the object eligible for garbage collection
				lua_settable ( state, -3 ); // save it in the table
			}
			lua_pop ( state, 1 ); //pop ref table
		}
	}
	
	// this will take the ref count to zero, but if the object hasn't been collected it *won't* get deleted
	// thanks to the override of ZLRefCountedObject::ZLRefCountedObjectBase_OnRelease ()
	object->Release ();
}

//----------------------------------------------------------------//
void MOAILuaObject::LuaRetain () {

	this->LuaRetain ( this );
}

//----------------------------------------------------------------//
void MOAILuaObject::LuaRetain ( MOAILuaObject* object ) {

	if ( !object ) return;
	object->Retain (); // strong ref

	MOAIScopedLuaState state = this->Get < MOAILuaRuntime >().State ();
	
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
		lua_pop ( state, 1 ); //pop ref table
	}
}

//----------------------------------------------------------------//
MOAILuaObject::MOAILuaObject () :
	mActiveUserdataCount ( 0 ),
	mPoolType ( NOT_IN_POOL ) {
	
	RTTI_BEGIN ( MOAILuaObject )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAILuaObject >)
		RTTI_EXTEND ( RTTIBase )
	RTTI_END

	MOAILuaRuntime& runtime = this->Get < MOAILuaRuntime >();

	if ( runtime.IsValid ()) {
		runtime.RegisterObject ( *this );
	}
}

//----------------------------------------------------------------//
MOAILuaObject::~MOAILuaObject () {

	MOAILuaObject::Unbind ( this, this->mUserdata );
}

//----------------------------------------------------------------//
void MOAILuaObject::MakeLuaBinding ( MOAILuaState& state ) {

	MOAILuaClass* type = this->GetLuaClass ();
	assert ( type );

	// stack:
	// -1: interface table
	// -2: member table
	// -3: ref table
	// -4: userdata

	int top 			= state.GetTop ();
	int memberTable 	= top - 1;
	int refTable 		= top - 2;

	// ref table gets gc and tostring
	if ( !this->IsSingleton ()) {
		lua_pushcfunction ( state, MOAILuaObject::_gc );
		lua_setfield ( state, refTable, "__gc" );
	}
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
	
	this->MOAILuaObject_DecorateLuaBinding ( state );
}

//----------------------------------------------------------------//
void MOAILuaObject::PrintTracking () {

	this->Get < MOAILuaRuntime >().PrintTracking ( *this );
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
	
	RTTIVisitorHistory history;
	RTTIVisitorIterator < MOAIAbstractLuaRegistrationVisitor > visitorIt = this->GetVisitors < MOAIAbstractLuaRegistrationVisitor >();
	for ( ; visitorIt; ++visitorIt ) {
		( *visitorIt ).RegisterLuaClass ( *this, history, state );
		assert ( history.CountVisits () == ( visitorIt.GetCount () + 1 ));
		assert ( history.CountRevisits () == 0 );
	}
}

//----------------------------------------------------------------//
void MOAILuaObject::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	RTTIVisitorHistory history;
	RTTIVisitorIterator < MOAIAbstractLuaRegistrationVisitor > visitorIt = this->GetVisitors < MOAIAbstractLuaRegistrationVisitor >();
	for ( ; visitorIt; ++visitorIt ) {
		( *visitorIt ).RegisterLuaFuncs ( *this, history, state );
		assert ( history.CountVisits () == ( visitorIt.GetCount () + 1 ));
		assert ( history.CountRevisits () == 0 );
	}
}

//----------------------------------------------------------------//
void MOAILuaObject::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {

	RTTIVisitorHistory history;
	RTTIVisitorIterator < MOAIAbstractLuaSerializationVisitor > visitorIt = this->GetVisitors < MOAIAbstractLuaSerializationVisitor >();
	for ( ; visitorIt; ++visitorIt ) {
		( *visitorIt ).SerializeIn ( *this, history, state, serializer );
		assert ( history.CountVisits () == ( visitorIt.GetCount () + 1 ));
		assert ( history.CountRevisits () == 0 );
	}
}

//----------------------------------------------------------------//
void MOAILuaObject::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {

	RTTIVisitorHistory history;
	RTTIVisitorIterator < MOAIAbstractLuaSerializationVisitor > visitorIt = this->GetVisitors < MOAIAbstractLuaSerializationVisitor >();
	for ( ; visitorIt; ++visitorIt ) {
		( *visitorIt ).SerializeOut ( *this, history, state, serializer );
		assert ( history.CountVisits () == ( visitorIt.GetCount () + 1 ));
		assert ( history.CountRevisits () == 0 );
	}
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
void MOAILuaObject::Unbind ( MOAILuaObject* object, MOAILuaWeakRef& userdata ) {

	MOAILuaRuntime& runtime = object->Get < MOAILuaRuntime >();

	if ( runtime.IsValid () && userdata ) {
		
		runtime.DeregisterObject ( *object );
		
		// TODO: change from both patrick's fork and the community branch; double check
		MOAIScopedLuaState state = runtime.State ();
		
		// clear out the gc
		userdata.PushRef ( state );
		
		runtime.PurgeUserdata ( state, -1 );
		
		if ( lua_getmetatable ( state, -1 )) {
			lua_pushnil ( state );
			lua_setfield ( state, -2, "__gc" );
			state.Pop ( 1 );
		}
					
		// and the ref table
		lua_pushnil ( state );
		lua_setmetatable ( state, -2 );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAILuaObject::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAILuaObject::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

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
void MOAILuaObject::MOAILuaObject_DecorateLuaBinding ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAILuaObject::MOAILuaObject_OnPooledRemit () {
}

//----------------------------------------------------------------//
void MOAILuaObject::ZLRefCountedObjectBase_OnRelease ( u32 refCount ) {

	if ( this->IsInPool () && ( refCount < 2 )) {
		if ( refCount == 0 ) {
			this->Retain ();
		}
		this->Get < MOAIPool >().Remit ( this );
	}
	
	// The engine is done with this object, so it's OK to delete
	// it if there is no connection to the Lua runtime. If there
	// is, then refcount can remain 0 and the object will be
	// collected by the Lua GC.

	if (( !this->IsBound ()) && ( refCount == 0 )) {
		// no Lua binding and no references, so
		// go ahead and kill this turkey
		delete this;
	}
}
