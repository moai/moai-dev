// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <aku/AKU.h>
#include <moaicore/MOAILuaObject.h>
#include <moaicore/MOAILuaState.h>
#include <moaicore/MOAILuaStateHandle.h>
#include <moaicore/MOAILuaRuntime.h>
#include <moaicore/MOAILuaRef.h>
#include <moaicore/MOAILuaState-impl.h>

#include <lstate.h>

#define DUMP_FORMAT "%p <%s> %s"

typedef STLSet < struct Table* > TableSet;

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
static void _dumpType ( lua_State* L, int idx, const char *name, bool verbose, TableSet& foundTables ) {

	MOAILuaState state ( L );

	const char *format = DUMP_FORMAT;

	idx = state.AbsIndex( idx );
	StkId tvalue = state->base + idx - 1;

	switch ( lua_type ( state, idx )) {

		case LUA_TBOOLEAN:

			USLog::Print ( format, tvalue, "bool", name );
			USLog::Print ( " = %s", lua_toboolean ( state, idx ) ? "true" : "false" );
			break;

		case LUA_TFUNCTION: {

			const char *funcType = iscfunction ( tvalue ) ? "C function" : "Lua function";

			USLog::Print ( format, clvalue ( tvalue ), funcType, name );
			break;
		}

		case LUA_TLIGHTUSERDATA:

			USLog::Print ( format, pvalue ( tvalue ), "pointer", name );
			break;

		case LUA_TNIL:

			USLog::Print ( format, tvalue, "nil", name );
			break;

		case LUA_TNONE:
			 // Intentionally do nothing--not even the line break.
			return;

		case LUA_TNUMBER:

			USLog::Print ( format, tvalue, "number", name );
			USLog::Print ( " = %f", lua_tonumber ( state, idx ));
			break;

		case LUA_TSTRING:

			USLog::Print ( format, rawtsvalue( tvalue ), "string", name );
			USLog::Print ( " = \"%s\"", lua_tostring ( state, idx ));
			break;

		case LUA_TTABLE: {

			struct Table* htable = hvalue( tvalue );

			if ( foundTables.contains ( htable )) {

				USLog::Print ( DUMP_FORMAT " (see above)", htable, "table", name );
				break;
			}
			else {

				foundTables.insert ( htable );

				USLog::Print ( format, htable, "table", name );

				if ( verbose ) {

					USLog::Print ( "\n" );
					lua_pushnil ( state );

					while ( lua_next ( state, idx ) ) {

						STLString elementName( name );
						elementName.append ( "." );
						elementName.append ( lua_tostring ( state, -2 ));
						_dumpType ( state, -1, elementName.c_str (), verbose, foundTables );
						lua_pop ( state, 1 );
					}
				}
			}
		}
			return; // suppress newline

		case LUA_TTHREAD:

			USLog::Print ( format, thvalue( tvalue ), "thread", name );
			break;

		case LUA_TUSERDATA:

			if ( lua_islightuserdata ( state, idx ) ) {
				
				USLog::Print ( format, lua_topointer ( state, idx ) , "light userdata", name );
			}
			else {

				USLog::Print ( format, lua_topointer( state, idx ), "userdata", name );

				if ( verbose ) {

					lua_getglobal ( state, "tostring" );
					lua_pushvalue ( state, idx );
					
					lua_pcall ( state, 1, 1, 0 );

					USLog::Print ( "\n\t%s", lua_tostring ( state, -1 ));
					state.Pop ( 1 );
				}
			}
			break;

		default:
			USLog::Print ( "*** Unexpected type: %d ***", lua_type ( state, idx ));
	}

	USLog::Print ( "\n" );
}

//----------------------------------------------------------------//
static void _dumpTypeByAddress ( lua_State* L, TValue* tvalue, const char *name, bool verbose, TableSet& foundTables ) {

	MOAILuaState state ( L );
	
	lua_lock ( L );
	setobj2s ( L, L->top, tvalue );
	L->top++;
	lua_unlock ( L );

	_dumpType ( L, -1, name, verbose, foundTables );
	lua_pop ( L, 1 );
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAILuaRuntime::_panic ( lua_State *L ) {

	MOAILuaState state ( L );
	state.PrintStackTrace ( USLog::CONSOLE, 1 );

#ifdef ANDROID
	USLog::Print ( "PANIC: unprotected error in call to Lua API (%s)\n", lua_tostring ( L, -1 ));
#else
	fprintf ( stderr, "PANIC: unprotected error in call to Lua API (%s)\n", lua_tostring ( L, -1 ));
#endif
	return 0;
}

//----------------------------------------------------------------//
void* MOAILuaRuntime::_tracking_alloc ( void *ud, void *ptr, size_t osize, size_t nsize ) {
	UNUSED ( ud );
	
	if ( MOAILuaRuntime::IsValid ()) {
		MOAILuaRuntime& self = MOAILuaRuntime::Get ();
		
		if ( nsize == 0 ) {
			self.mTotalBytes -= osize;
			free ( ptr );
			return NULL;
		}

		if ( self.mAllocLogEnabled ) {
			printf ( "Lua alloc: %d\n", ( int )nsize );
		}

		self.mTotalBytes -= osize;
		self.mTotalBytes += nsize;
		return realloc ( ptr, nsize );
	}
	
	if ( nsize == 0 ) {
		free ( ptr );
		return NULL;
	}

	return realloc ( ptr, nsize );
}

//================================================================//
// MOAILuaRuntime Lua API
//================================================================//

//----------------------------------------------------------------//
static int _debugCall ( lua_State* L ) {

	MOAILuaState state ( L );

	state.DebugCall ( 0, 0 );

	return 0;
}

//----------------------------------------------------------------//
static int _deleteLuaData ( lua_State* L ) {

	MOAILuaState state ( L );

	MOAILuaObject* self = state.GetLuaObject < MOAILuaObject >( 1, false );
	delete self;

	return 0;
}

//----------------------------------------------------------------//
static int _dump ( lua_State* L ) {

	MOAILuaState state ( L );

	if ( !state.CheckParams ( 1, "S." )) return 0;

	cc8* name = state.GetValue < cc8* >( 1, "" );
	bool verbose = state.GetValue < bool >( 3, true );

	TableSet foundTables;
	_dumpType ( state, 2, name, verbose, foundTables );

	return 0;
}

//----------------------------------------------------------------//
static int _dumpStack ( lua_State* L ) {

	MOAILuaState state ( L );

	bool verbose = state.GetValue < bool >( 1, true );
	int idx = 0;

	TableSet foundTables;
	for ( TValue* tvalue = state->stack; tvalue < state->top; ++tvalue ) {

		USLog::Print ( "stack [ %d ] ", idx++ );
		_dumpTypeByAddress ( state, tvalue, "", verbose, foundTables );
	}
	return 0;
}

//----------------------------------------------------------------//
static int _traceback ( lua_State *L ) {

	MOAILuaState state ( L );
		
	cc8* msg = NULL;
	if ( lua_isstring ( L, 1 )) {  // 'message' a string?
		
		msg = lua_tostring ( L, 1 );
	}
	
	if ( MOAILuaRuntime::Get ().GetCustomTraceback ()) {
		
		state.Push ( MOAILuaRuntime::Get ().GetCustomTraceback ());
		state.Push ( msg );
		
		state.DebugCall ( 1, 0 );
	}
	else if ( msg ) {

		USLog::Print ( "%s\n", msg );
	}
	
	state.PrintStackTrace ( USLog::CONSOLE, 1 );
	
	return 0;
}

//----------------------------------------------------------------//
static int _register ( lua_State* L ) {

	STLString name = lua_tostring ( L, 1 );

	luaL_Reg regTable [] = {
		{ "debugCall",		_debugCall },
		{ "deleteLuaData",	_deleteLuaData },
		{ "dump",			_dump },
		{ "dumpStack",		_dumpStack },
		{ "traceback",		_traceback },
		{ NULL, NULL }
	};

	luaL_register ( L, name, regTable );
	return 1;
}

//================================================================//
// MOAILuaRuntime
//================================================================//

//----------------------------------------------------------------//
void MOAILuaRuntime::BuildHistogram ( HistMap& histogram ) {

	HistSet::iterator histSetIt = this->mHistSet.begin ();
	for ( ; histSetIt != this->mHistSet.end (); ++histSetIt ) {
	
		MOAILuaObject* obj = *histSetIt;
		cc8* name = obj->TypeName ();
	
		if ( !histogram.contains ( name )) {
			histogram [ name ] = 1;
		}
		else {
			histogram [ name ]++;
		}
	}
}

//----------------------------------------------------------------//
void MOAILuaRuntime::ClearObjectStackTrace ( MOAILuaObject* object ) {

	if ( object ) {
		this->mLeaks.erase ( object );
	}
}

//----------------------------------------------------------------//
void MOAILuaRuntime::Close () {

	if ( this->mMainState ) {
		this->mWeakRefTable.Clear ();
		lua_close ( this->mMainState );
		this->mMainState = 0;
	}
}

//----------------------------------------------------------------//
void MOAILuaRuntime::DeregisterObject ( MOAILuaObject& object ) {

	this->mObjectCount--;
	
	if ( this->mHistogramEnabled ) {
		this->mHistSet.erase ( &object );
	}
}

//----------------------------------------------------------------//
void MOAILuaRuntime::EnableHistogram ( bool enable ) {

	this->mHistogramEnabled = enable;
	
	if ( !enable ) {
		this->mHistSet.clear ();
	}
}

//----------------------------------------------------------------//
void MOAILuaRuntime::EnableLeakTracking ( bool enable ) {

	this->mLeakTrackingEnabled = enable;
}

//----------------------------------------------------------------//
// This beast will walk through all tables and functions accessible in the
// current lua state and print a reference line for each one found to help
// track who is pointing to it.
void MOAILuaRuntime::FindAndPrintLuaRefs ( int idx, cc8* prefix, FILE *f, const LeakPtrList& objects ) {

	lua_State* L = this->mMainState;

	// Convert to absolute index
	if ( idx < 0 ) {
		idx = lua_gettop(L) + idx + 1;
	}
	
	// Check if the item at the top of the stack has been traversed yet.
	lua_pushvalue ( L, -1 );
	lua_gettable ( L, idx );
	if( lua_type ( L, -1 ) != LUA_TNIL ) {
		// It has, let's bail.
		lua_pop ( L, 1 ); // Clean our 'true'
		return;
	}
	lua_pop(L, 1); // Remove the nil
	
	int tt = lua_type ( L, -1 );
	if( tt == LUA_TTABLE ) {
//		printf("finding refs in: %s\n", prefix);
		// It hasn't been visited, so mark it in our traversal set
		lua_pushvalue ( L, -1 ); // Push table as key
		lua_pushboolean ( L, true );
		lua_settable ( L, idx );
		
		lua_pushnil ( L );  // first key
		while ( lua_next ( L, -2 ) != 0 ) {
			
			// use the 'key' (at index -2) and 'value' (at index -1)
			STLString key;
			
			if ( lua_type ( L, -2) == LUA_TSTRING ) {
				if ( MOAILuaRuntime::IsLuaIdentifier ( lua_tostring ( L, -2 ))) {
					key.write ( "%s.%s", prefix, lua_tostring ( L, -2 ));
				}
				else {
					// TODO: escape '\"'
					key.write ( "%s[\"%s\"]", prefix, lua_tostring ( L, -2 ));
				}
			}
			else {
				// stringify key
				lua_pushstring ( L, "tostring" );
				lua_gettable ( L, LUA_GLOBALSINDEX );
				lua_pushvalue ( L, -3 );
				lua_call ( L, 1, 1 );
				
				key.write ( "%s[%s]", prefix, lua_tostring ( L, -1 ));
				// Pop stringified key
				lua_pop ( L, 1 );
			}
			
			this->FindAndPrintLuaRefs ( idx, key.c_str (), f, objects );

			// removes 'value'; keeps 'key' for next iteration
			lua_pop ( L, 1 );
		}
		
		// Check its metatable (if it has one)
		if ( lua_getmetatable ( L, -1 )) {
			STLString key;
			key.write ( "%s~mt", prefix );
			this->FindAndPrintLuaRefs ( idx, key.c_str(), f, objects );
			lua_pop ( L, 1 ); // Pop metatable
		}
	}
	else if ( tt == LUA_TFUNCTION ) {
//		printf("finding refs in: %s\n", prefix);
		// It hasn't been visited, so mark it in our tarversal set
		lua_pushvalue ( L, -1 ); // Push table as key
		lua_pushboolean ( L, true );
		lua_settable ( L, idx );
		
		const char *upname;
		for ( int i = 1; ( upname = lua_getupvalue ( L, -1, i )) != NULL; ++i ) {
			STLString key;
			key.write ( "%s(%s)", prefix, upname );
			this->FindAndPrintLuaRefs ( idx, key.c_str(), f, objects );
			// Pop the upvalue
			lua_pop ( L, 1 );
		}
	}
	else if ( tt == LUA_TUSERDATA ) {
		// It hasn't been visited, so mark it in our traversal set
		lua_pushvalue ( L, -1 ); // Push table as key
		lua_pushboolean ( L, true );
		lua_settable ( L, idx );

		MOAILuaState state ( L );
		void *ud = state.GetPtrUserData ( -1 );
		for ( LeakPtrList::const_iterator i = objects.begin (); i != objects.end (); ++i ) {
			if( *i == ud ) {
				fprintf ( f, "\tLua Ref: %s = %s <%p>\n", prefix, ( *i )->TypeName (), ud );
//				if ( strcmp((*i)->TypeName(), "MOAICoroutine") == 0 ) {
//					MOAICoroutine *t = (MOAICoroutine*)ud;
//				}
			}
		}
		
		// Check its metatable (if it has one)
		if ( lua_getmetatable ( L, -1 )) {
			STLString key;
			key.write ( "%s~mt", prefix );
			this->FindAndPrintLuaRefs ( idx, key.c_str (), f, objects );
			lua_pop ( L, 1 ); // Pop metatable
		}
	}
}

//----------------------------------------------------------------//
void MOAILuaRuntime::ForceGarbageCollection () {

	lua_State* L = this->mMainState;

	// Make sure that anything that can be collected, is. Note: we collect
	// more than once because of this scary snippet:
	//   "When Lua collects a full userdata with a gc metamethod, Lua
	//    calls the metamethod and marks the userdata as finalized. When
	//    this userdata is collected again then Lua frees its corresponding
	//    memory."
	
	// collect until no more changes in memory use or object count are registered
	bool more = true;
	while ( more ) {
	
		size_t b0 = this->mTotalBytes;
		size_t c0 = this->mObjectCount;
		
		lua_gc ( L, LUA_GCCOLLECT, 0 );
		
		size_t b1 = this->mTotalBytes;
		size_t c1 = this->mObjectCount;
		
		if (( b0 == b1 ) && ( c0 == c1 )) {
			more = false;
		}
	}
}

//----------------------------------------------------------------//
size_t MOAILuaRuntime::GetMemoryUsage() {
	return this->mTotalBytes;
}

//----------------------------------------------------------------//
MOAILuaRef& MOAILuaRuntime::GetCustomTraceback () {
	return this->mCustomTraceback;
}

//----------------------------------------------------------------//
MOAILuaState& MOAILuaRuntime::GetMainState () {
	return this->mMainState;
}

//----------------------------------------------------------------//
bool MOAILuaRuntime::IsLuaIdentifier ( const char *str ) {
	const char *p = str;
	while ( *p != '\0' && ( isalnum(*p) || *p == '_' )) {
		p++;
	}
	return p > str && *p == '\0';
}

//----------------------------------------------------------------//
bool MOAILuaRuntime::IsOpen () {

	return ( this->mMainState != 0 );
}

//----------------------------------------------------------------//
void MOAILuaRuntime::LoadLibs ( cc8* runtimeLibName ) {

	// Load the standard Lua libs
	luaL_openlibs ( this->mMainState );

	this->RegisterModule ( runtimeLibName, _register, true );
	
	this->mMainState.Push ( _traceback );
	this->mTraceback.SetStrongRef ( this->mMainState, -1 );
	this->mMainState.Pop ( 1 );
}

//----------------------------------------------------------------//
MOAILuaStateHandle MOAILuaRuntime::Open () {

	if ( this->IsOpen ()) {
		this->Close ();
	}

	// open the main state
	this->mMainState = lua_newstate ( _tracking_alloc, NULL );
	lua_atpanic ( this->mMainState, &_panic );

	// set up the ref tables
	this->mWeakRefTable.InitWeak ();
	this->mStrongRefTable.InitStrong ();
	
	return MOAILuaStateHandle ( this->mMainState );
}

//----------------------------------------------------------------//
void MOAILuaRuntime::PushHistogram ( MOAILuaState& state ) {
	
	if ( !this->mHistogramEnabled ) {
		lua_pushnil ( state );
		return;
	}
	
	lua_newtable ( state );
	
	HistMap histogram;
	this->BuildHistogram ( histogram );
	
	HistMap::iterator histogramIt = histogram.begin ();
	for ( ; histogramIt != histogram.end (); ++histogramIt ) {
	
		const STLString& name = histogramIt->first;
		size_t count = histogramIt->second;
		
		lua_pushstring ( state, name );
		lua_pushnumber ( state, count );
		lua_settable ( state, -3 );
	}
}

//----------------------------------------------------------------//
void MOAILuaRuntime::RegisterModule ( cc8* name, lua_CFunction loader, bool autoLoad ) {

	this->mMainState.RegisterModule ( loader, name, autoLoad );
}

//----------------------------------------------------------------//
void MOAILuaRuntime::RegisterObject ( MOAILuaObject& object ) {

	this->mObjectCount++;
	
	if ( this->mHistogramEnabled ) {
		this->mHistSet.affirm ( &object );
	}
}

//----------------------------------------------------------------//
void MOAILuaRuntime::ReportHistogram ( FILE *f ) {

	if ( !this->mHistogramEnabled ) return;
	
	HistMap histogram;
	this->BuildHistogram ( histogram );
	
	fprintf ( f, "tracking %d of %d allocated MOAIObjects\n", ( int )this->mHistSet.size (), ( int )this->mObjectCount );
	
	size_t totalTracked = this->mHistSet.size ();
	
	HistMap::iterator histogramIt = histogram.begin ();
	for ( ; histogramIt != histogram.end (); ++histogramIt ) {
	
		const STLString& name = histogramIt->first;
		size_t count = histogramIt->second;
		float percent = (( float )count / ( float )totalTracked ) * 100.0f;
	
		fprintf ( f, "%-32.32s %d (%.2f%% of %d)\n", name.str (), ( int )count, percent, ( int )totalTracked );
	}
}

//----------------------------------------------------------------//
void MOAILuaRuntime::ReportLeaksFormatted ( FILE *f ) {

	this->ForceGarbageCollection ();

	lua_State* L = this->mMainState;
		
	// First, correlate leaks by identical stack traces.
	LeakStackMap stacks;
	
	for ( LeakMap::const_iterator i = this->mLeaks.begin (); i != this->mLeaks.end (); ++i ) {
		stacks [ i->second ].push_back ( i->first );
	}
	
	fprintf ( f, "-- BEGIN LUA OBJECT LEAKS --\n" );
	
	// Then, print out each unique allocation spot along with all references
	// (including multiple references) followed by the alloction stack
	int top = lua_gettop ( L );
	UNUSED ( top );
	for ( LeakStackMap::const_iterator i = stacks.begin (); i != stacks.end (); ++i ) {
		
		const LeakPtrList& list = i->second;
		
		MOAILuaObject *o = list.front ();
		fprintf ( f, "Allocation: %lu x %s\n", list.size (), o->TypeName ()); 
		for( LeakPtrList::const_iterator j = list.begin (); j != list.end (); ++j ) {
			fprintf ( f, "\t(%6d) %p\n", ( *j )->GetRefCount (), *j );
		}
		// A table to use as a traversal set.
		lua_newtable ( L );
		// And the table to use as seed
		lua_pushvalue ( L, LUA_GLOBALSINDEX );
		
		this->FindAndPrintLuaRefs ( -2, "_G", f, list );
		
		lua_pop ( L, 2 ); // Pop the 'done' set and our globals table
		fputs ( i->first.c_str (), f );
		fputs ( "\n", f );
		fflush ( f );
	}
	assert ( top == lua_gettop ( L ));
	fprintf ( f, "-- END LUA LEAKS --\n" );
}

//----------------------------------------------------------------//
void MOAILuaRuntime::ReportLeaksRaw ( FILE *f ) {

	this->ForceGarbageCollection ();
	
	fprintf ( f, "-- LUA OBJECT LEAK REPORT ------------\n" );
	u32 count = 0;
	
	for ( LeakMap::const_iterator i = this->mLeaks.begin () ; i != this->mLeaks.end (); ++i ) {
		fputs ( i->second.c_str (), f );
		count++;
	}
	fprintf ( f, "-- END LEAK REPORT (Total Objects: %d) ---------\n", count );
}

//----------------------------------------------------------------//
void MOAILuaRuntime::ResetLeakTracking () {

	this->mLeaks.clear ();
}

//----------------------------------------------------------------//
void MOAILuaRuntime::SetObjectStackTrace ( MOAILuaObject* object ) {

	if ( object && this->mLeakTrackingEnabled ) {
	
		STLString trace = this->mMainState.GetStackTrace ( 1 );
		this->mLeaks [ object ] = trace;
	}
}

//----------------------------------------------------------------//
void MOAILuaRuntime::SetPath ( cc8* path ) {

	int top = lua_gettop ( this->mMainState );

	lua_getglobal ( this->mMainState, "package" );
	int packageIdx = lua_gettop ( this->mMainState );

	lua_pushstring ( this->mMainState, "path" );
	lua_pushstring ( this->mMainState, path );
	lua_settable ( this->mMainState, packageIdx );

	lua_settop ( this->mMainState, top );
}

//----------------------------------------------------------------//
MOAILuaStateHandle MOAILuaRuntime::State () {

	return MOAILuaStateHandle ( this->mMainState );
}

//----------------------------------------------------------------//
MOAILuaRuntime::MOAILuaRuntime () :
	mHistogramEnabled ( false ),
	mLeakTrackingEnabled ( false ),
	mTotalBytes ( 0 ),
	mObjectCount ( 0 ),
	mAllocLogEnabled ( false ) {
}

//----------------------------------------------------------------//
MOAILuaRuntime::~MOAILuaRuntime () {

	if ( this->mMainState ) {
		// run a full cycle of the garbage collector here in case any Lua bound objects
		// need to send callbacks on destruction
		lua_gc ( this->mMainState, LUA_GCCOLLECT, 0 );
		lua_close ( this->mMainState );
	}
	this->mMainState = 0;
}
