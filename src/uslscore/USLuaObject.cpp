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


typedef std::map<USLuaObject*,STLString> LeakMap;
typedef std::vector<USLuaObject*> LeakPtrList;
typedef std::map<STLString, LeakPtrList> LeakStackMap;

static bool gLeakTrackingEnabled;
static LeakMap gLeaks;

static bool isluaident(const char *str)
{
	const char *p = str;
	while ( *p != '\0' && ( isalnum(*p) || *p == '_' ) )
	{
		p++;
	}
	return p > str && *p == '\0';
}

// This beast will walk through all tables and functions accessible in the
// current lua state and print a reference line for each one found to help
// track who is pointing to it.
static void FindAndPrintLuaRefs(lua_State *L, int idx, cc8* prefix, FILE *f, const LeakPtrList& objects)
{
	// Convert to absolute index
	if ( idx < 0 )
		idx = lua_gettop(L) + idx + 1;
	
	// Check if the item at the top of the stack has been traversed yet.
	lua_pushvalue(L, -1);
	lua_gettable(L, idx);
	if( lua_type(L, -1) != LUA_TNIL )
	{
		// It has, let's bail.
		lua_pop(L, 1); // Clean our 'true'
		return;
	}
	lua_pop(L, 1); // Remove the nil
	
	int tt = lua_type(L, -1);
	if( tt == LUA_TTABLE )
	{
//		printf("finding refs in: %s\n", prefix);
		// It hasn't been visited, so mark it in our tarversal set
		lua_pushvalue(L, -1); // Push table as key
		lua_pushboolean(L, true);
		lua_settable(L, idx);
		
		lua_pushnil(L);  // first key
		while( lua_next(L, -2) != 0 )
		{
			// use the 'key' (at index -2) and 'value' (at index -1)
			STLString key;
			
			if (lua_type(L, -2) == LUA_TSTRING )
			{
				if( isluaident(lua_tostring(L, -2)) )
					key.write("%s.%s", prefix, lua_tostring(L, -2));
				else // TODO: escape '\"'
					key.write("%s[\"%s\"]", prefix, lua_tostring(L, -2));
			}
			else
			{
				// stringify key
				lua_pushstring(L, "tostring");
				lua_gettable(L, LUA_GLOBALSINDEX);
				lua_pushvalue(L, -3);
				lua_call(L, 1, 1);
				
				key.write("%s[%s]", prefix, lua_tostring(L, -1));
				// Pop stringified key
				lua_pop(L, 1);
			}
			
			FindAndPrintLuaRefs(L, idx, key.c_str(), f, objects);

			// removes 'value'; keeps 'key' for next iteration
			lua_pop(L, 1);
		}
		
		// Check its metatable (if it has one)
		if( lua_getmetatable(L, -1) )
		{
			STLString key;
			key.write("%s~mt", prefix);
			FindAndPrintLuaRefs(L, idx, key.c_str(), f, objects);
			lua_pop(L, 1); // Pop metatable
		}
	}
	else if( tt == LUA_TFUNCTION )
	{
//		printf("finding refs in: %s\n", prefix);
		// It hasn't been visited, so mark it in our tarversal set
		lua_pushvalue(L, -1); // Push table as key
		lua_pushboolean(L, true);
		lua_settable(L, idx);
		
		const char *upname;
		for (int i = 1; (upname = lua_getupvalue(L, -1, i)) != NULL; ++i)
		{
			STLString key;
			key.write("%s(%s)", prefix, upname);
			FindAndPrintLuaRefs(L, idx, key.c_str(), f, objects);
			// Pop the upvalue
			lua_pop(L, 1);
		}
	}
	else if( tt == LUA_TUSERDATA )
	{
		// It hasn't been visited, so mark it in our tarversal set
		lua_pushvalue(L, -1); // Push table as key
		lua_pushboolean(L, true);
		lua_settable(L, idx);

		USLuaState state ( L );
		void *ud = state.GetPtrUserData(-1);
		for (LeakPtrList::const_iterator i = objects.begin(); i != objects.end(); ++i)
		{
			if( *i == ud )
			{
				fprintf(f, "\tLua Ref: %s = %s <%p>\n", prefix, (*i)->TypeName(), ud);
//				if ( strcmp((*i)->TypeName(), "MOAIThread") == 0 )
//				{
//					MOAIThread *t = (MOAIThread*)ud;
//					
//				}
			}
		}
		
		// Check its metatable (if it has one)
		if( lua_getmetatable(L, -1) )
		{
			STLString key;
			key.write("%s~mt", prefix);
			FindAndPrintLuaRefs(L, idx, key.c_str(), f, objects);
			lua_pop(L, 1); // Pop metatable
		}
	}
}

void USLuaObject::ReportLeaks( FILE *f, bool clearAfter )
{
	lua_State *L = USLuaRuntime::Get().State();

	// Make sure that anything that can be collected, is. Note: we collect
	// more than once because of this scary snippet:
	//   "When Lua collects a full userdata with a gc metamethod, Lua
	//    calls the metamethod and marks the userdata as finalized. When
	//    this userdata is collected again then Lua frees its corresponding
	//    memory."
	
	lua_gc(L, LUA_GCCOLLECT, 0);
	lua_gc(L, LUA_GCCOLLECT, 0);
	lua_gc(L, LUA_GCCOLLECT, 0);
	lua_gc(L, LUA_GCCOLLECT, 0);
	lua_gc(L, LUA_GCCOLLECT, 0);
	lua_gc(L, LUA_GCCOLLECT, 0);
	
#if 0
	
	// Old way was just a raw dump (but it was much faster)
	fprintf(f, "-- MOAI LEAK REPORT ------------\n");
	u32 count = 0;
	
	for( LeakMap::const_iterator i = gLeaks.begin() ; i != gLeaks.end(); ++i )
	{
		fputs(i->second.c_str(), f);
		count++;
	}
	
	fprintf(f, "-- END LEAK REPORT (Total Objects: %lu) ---------\n", count);

#else
	
	// First, correlate leaks by identical stack traces.
	
	LeakStackMap stacks;
	
	for( LeakMap::const_iterator i = gLeaks.begin() ; i != gLeaks.end(); ++i )
		stacks[i->second].push_back(i->first);
	
	fprintf(f, "-- BEGIN MOAI LEAKS --\n");
	// Then, print out each unique allocation spot along with all references
	// (including multiple references) followed by the alloction stack
	int top = lua_gettop(L);
	for( LeakStackMap::const_iterator i = stacks.begin() ; i != stacks.end(); ++i )
	{
		const LeakPtrList& list = i->second;
		
		USLuaObject *o = list.front();
		fprintf(f, "Allocation: %lu x %s\n", list.size(), o->TypeName()); 
		for( LeakPtrList::const_iterator j = list.begin(); j != list.end(); ++j )
		{
			fprintf(f, "\t(%6d) %p\n", (*j)->GetRefCount(), *j);
		}
		// A table to use as a traversal set.
		lua_newtable(L);
		// And the table to use as seed
		lua_pushvalue(L, LUA_GLOBALSINDEX);
		
		FindAndPrintLuaRefs(L, -2, "_G", f, list);
		
		lua_pop(L, 2); // Pop the 'done' set and our globals table
		fputs(i->first.c_str(), f);
		fputs("\n", f);
		fflush(f);
	}
	assert( top == lua_gettop(L) );
	fprintf(f, "-- END MOAI LEAKS --\n");
	
	
#endif
	
	if( clearAfter )
	{
		gLeaks.clear();
	}
}

void USLuaObject::SetLeakTrackingEnabled( bool enabled )
{
	gLeakTrackingEnabled = enabled;
}

void USLuaObject::CallCreationHook( USLuaState &state )
{
	if( gLeakTrackingEnabled )
	{
		gLeaks[this] = state.GetStackTrace(1);
	}
}


//----------------------------------------------------------------//
int USLuaObject::_gc ( lua_State* L ) {

	USLuaState state ( L );
	
	USLuaObject* data = ( USLuaObject* )state.GetPtrUserData ( 1 );

	bool cleanup = data->mUserdata.IsWeak ();
	data->mUserdata.Clear ();
	
	LeakMap::iterator it = gLeaks.find(data);
	if( it != gLeaks.end() )
		gLeaks.erase(it);

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

	USLog::Print ( "%p <%s> %s", this, this->TypeName (), this->ToString ().c_str ());
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

	// This shouldn't be necessary, but let's be paranoid.
	LeakMap::iterator it = gLeaks.find(this);
	if( it != gLeaks.end() )
		gLeaks.erase(it);
	
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

