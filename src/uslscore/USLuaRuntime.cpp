// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USLuaObject.h>
#include <uslscore/USLuaState.h>
#include <uslscore/USLuaStateHandle.h>
#include <uslscore/USLuaRuntime.h>
#include <uslscore/USLuaRef.h>
#include <uslscore/USLog.h>
#include <uslscore/USRtti.h>

#include <uslscore/USLuaState-impl.h>

#include <uslscore/STLSet.h>
#include <lstate.h>

#define DUMP_FORMAT "%p <%s> %s"

typedef STLSet < struct Table* > TableSet;

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
static void _dumpType ( lua_State* L, int idx, const char *name, bool verbose, TableSet& foundTables ) {

	USLuaState state ( L );

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

	USLuaState state ( L );
	
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
int USLuaRuntime::_panic ( lua_State *L ) {
	UNUSED ( L );

	fprintf ( stderr, "PANIC: unprotected error in call to Lua API (%s)\n", lua_tostring ( L, -1 ));
	return 0;
}

//----------------------------------------------------------------//
void* USLuaRuntime::_tracking_alloc ( void *ud, void *ptr, size_t osize, size_t nsize ) {
	UNUSED ( ud );
	
	if ( USLuaRuntime::IsValid ()) {
		USLuaRuntime& self = USLuaRuntime::Get ();
	
		if ( nsize == 0 ) {
			self.mTotalBytes -= osize;
			free ( ptr );
			return NULL;
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
// USLuaRuntime Lua API
//================================================================//

//----------------------------------------------------------------//
static int _debugCall ( lua_State* L ) {

	USLuaState state ( L );

	state.DebugCall ( 0, 0 );

	return 0;
}

//----------------------------------------------------------------//
static int _deleteLuaData ( lua_State* L ) {

	USLuaState state ( L );

	USLuaObject* self = state.GetLuaObject < USLuaObject >( 1 );
	delete self;

	return 0;
}

//----------------------------------------------------------------//
static int _dump ( lua_State* L ) {

	USLuaState state ( L );

	if ( !state.CheckParams ( 1, "S." )) return 0;

	cc8* name = state.GetValue < cc8* >( 1, "" );
	bool verbose = state.GetValue < bool >( 3, true );

	TableSet foundTables;
	_dumpType ( state, 2, name, verbose, foundTables );

	return 0;
}

//----------------------------------------------------------------//
static int _dumpStack ( lua_State* L ) {

	USLuaState state ( L );

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

	if ( lua_isstring ( L, 1 )) {  // 'message' a string?
	
		cc8* msg = lua_tostring ( L, 1 );
		USLog::Print ( "%s\n", msg );
	}
	
	USLuaState state ( L );
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
// USLuaRuntime
//================================================================//

//----------------------------------------------------------------//
void USLuaRuntime::Close () {

	if ( this->mMainState ) {
		this->mWeakRefTable.Clear ();
		lua_close ( this->mMainState );
		this->mMainState = 0;
	}
}

//----------------------------------------------------------------//
void USLuaRuntime::EnableLeakTracking ( bool enable ) {

	this->mLeakTrackingEnabled = enable;
}

//----------------------------------------------------------------//
// This beast will walk through all tables and functions accessible in the
// current lua state and print a reference line for each one found to help
// track who is pointing to it.
void USLuaRuntime::FindAndPrintLuaRefs ( int idx, cc8* prefix, FILE *f, const LeakPtrList& objects ) {

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
				if ( USLuaRuntime::IsLuaIdentifier ( lua_tostring ( L, -2 ))) {
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

		USLuaState state ( L );
		void *ud = state.GetPtrUserData ( -1 );
		for ( LeakPtrList::const_iterator i = objects.begin (); i != objects.end (); ++i ) {
			if( *i == ud ) {
				fprintf ( f, "\tLua Ref: %s = %s <%p>\n", prefix, ( *i )->TypeName (), ud );
//				if ( strcmp((*i)->TypeName(), "MOAIThread") == 0 ) {
//					MOAIThread *t = (MOAIThread*)ud;
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
void USLuaRuntime::ForceGarbageCollection () {

	lua_State* L = this->mMainState;

	// Make sure that anything that can be collected, is. Note: we collect
	// more than once because of this scary snippet:
	//   "When Lua collects a full userdata with a gc metamethod, Lua
	//    calls the metamethod and marks the userdata as finalized. When
	//    this userdata is collected again then Lua frees its corresponding
	//    memory."
	lua_gc ( L, LUA_GCCOLLECT, 0 );
	lua_gc ( L, LUA_GCCOLLECT, 0 );
	lua_gc ( L, LUA_GCCOLLECT, 0 );
	lua_gc ( L, LUA_GCCOLLECT, 0 );
	lua_gc ( L, LUA_GCCOLLECT, 0 );
	lua_gc ( L, LUA_GCCOLLECT, 0 );
}

//----------------------------------------------------------------//
size_t USLuaRuntime::GetMemoryUsage() {
	return this->mTotalBytes;
}

//----------------------------------------------------------------//
bool USLuaRuntime::IsLuaIdentifier ( const char *str ) {
	const char *p = str;
	while ( *p != '\0' && ( isalnum(*p) || *p == '_' )) {
		p++;
	}
	return p > str && *p == '\0';
}

//----------------------------------------------------------------//
bool USLuaRuntime::IsOpen () {

	return ( this->mMainState != 0 );
}

//----------------------------------------------------------------//
void USLuaRuntime::LoadLibs ( cc8* runtimeLibName ) {

	// Load the standard Lua libs
	luaL_openlibs ( this->mMainState );

	this->RegisterModule ( runtimeLibName, _register, true );
	
	this->mMainState.Push ( _traceback );
	this->mTraceback.SetRef ( this->mMainState, -1, false );
	this->mMainState.Pop ( 1 );
}

//----------------------------------------------------------------//
USLuaStateHandle USLuaRuntime::Open () {

	if ( this->IsOpen ()) {
		this->Close ();
	}

	// open the main state
	this->mMainState = lua_newstate ( _tracking_alloc, NULL );
	lua_atpanic ( this->mMainState, &_panic );

	// set up the ref tables
	this->mWeakRefTable.InitWeak ();
	this->mStrongRefTable.InitStrong ();
	
	return USLuaStateHandle ( this->mMainState );
}

//----------------------------------------------------------------//
void USLuaRuntime::RegisterModule ( cc8* name, lua_CFunction loader, bool autoLoad ) {

	this->mMainState.RegisterModule ( name, loader, autoLoad );
}

//----------------------------------------------------------------//
void USLuaRuntime::ReportLeaksFormatted ( FILE *f ) {

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
	for ( LeakStackMap::const_iterator i = stacks.begin (); i != stacks.end (); ++i ) {
		
		const LeakPtrList& list = i->second;
		
		USLuaObject *o = list.front ();
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
void USLuaRuntime::ReportLeaksRaw ( FILE *f ) {

	this->ForceGarbageCollection ();
	
	fprintf ( f, "-- LUA OBJECT LEAK REPORT ------------\n" );
	u32 count = 0;
	
	for ( LeakMap::const_iterator i = this->mLeaks.begin () ; i != this->mLeaks.end (); ++i ) {
		fputs ( i->second.c_str (), f );
		count++;
	}
	fprintf ( f, "-- END LEAK REPORT (Total Objects: %lu) ---------\n", count );
}

//----------------------------------------------------------------//
void USLuaRuntime::ResetLeakTracking () {

	this->mLeaks.clear ();
}

//----------------------------------------------------------------//
void USLuaRuntime::SetObjectStackTrace ( USLuaObject* object, cc8* trace ) {

	if ( object ) {
		if ( trace ) {
			if ( this->mLeakTrackingEnabled ) {
				this->mLeaks [ object ] = trace;
			}
		}
		else {
			this->mLeaks.erase ( object );
		}
	}
}

//----------------------------------------------------------------//
void USLuaRuntime::SetPath ( cc8* path ) {

	int top = lua_gettop ( this->mMainState );

	lua_getglobal ( this->mMainState, "package" );
	int packageIdx = lua_gettop ( this->mMainState );

	lua_pushstring ( this->mMainState, "path" );
	lua_pushstring ( this->mMainState, path );
	lua_settable ( this->mMainState, packageIdx );

	lua_settop ( this->mMainState, top );
}

//----------------------------------------------------------------//
USLuaStateHandle USLuaRuntime::State () {

	return USLuaStateHandle ( this->mMainState );
}

//----------------------------------------------------------------//
USLuaRuntime::USLuaRuntime () :
	mLeakTrackingEnabled ( false ),
	mTotalBytes ( 0 ) {
}

//----------------------------------------------------------------//
USLuaRuntime::~USLuaRuntime () {

	if ( this->mMainState ) {
		// run a full cycle of the garbage collector here in case any Lua bound objects
		// need to send callbacks on destruction
		lua_gc ( this->mMainState, LUA_GCCOLLECT, 0 );
		lua_close ( this->mMainState );
	}
	this->mMainState = 0;
}
