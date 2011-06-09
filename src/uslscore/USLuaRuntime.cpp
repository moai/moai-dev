// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USLuaObject.h>
#include <uslscore/USLuaState.h>
#include <uslscore/USLuaStateHandle.h>
#include <uslscore/USLuaRuntime.h>
#include <uslscore/USLuaRef.h>
#include <uslscore/USRtti.h>

#include <uslscore/USLuaState-impl.h>

#include <uslscore/STLSet.h>
#include <lstate.h>

//================================================================//
// local
//================================================================//

#define DUMP_FORMAT "%p <%s> %s"
typedef STLSet < struct Table* > TableSet;

//----------------------------------------------------------------//
static void dumpType ( lua_State* L, int idx, const char *name, bool verbose, TableSet& foundTables ) {

	USLuaState state ( L );

	const char *format = DUMP_FORMAT;

	idx = state.AbsIndex( idx );
	StkId tvalue = state->base + idx - 1;

	switch ( lua_type ( state, idx )) {

		case LUA_TBOOLEAN:

			printf ( format, tvalue, "bool", name );
			printf ( " = %s", lua_toboolean ( state, idx ) ? "true" : "false" );
			break;

		case LUA_TFUNCTION: {

			const char *funcType = iscfunction ( tvalue ) ? "C function" : "Lua function";

			printf ( format, clvalue ( tvalue ), funcType, name );
			break;
		}

		case LUA_TLIGHTUSERDATA:

			printf ( format, pvalue ( tvalue ), "pointer", name );
			break;

		case LUA_TNIL:

			printf ( format, tvalue, "nil", name );
			break;

		case LUA_TNONE:
			 // Intentionally do nothing--not even the line break.
			return;

		case LUA_TNUMBER:

			printf ( format, tvalue, "number", name );
			printf ( " = %f", lua_tonumber ( state, idx ));
			break;

		case LUA_TSTRING:

			printf ( format, rawtsvalue( tvalue ), "string", name );
			printf ( " = \"%s\"", lua_tostring ( state, idx ));
			break;

		case LUA_TTABLE: {

			struct Table* htable = hvalue( tvalue );

			if ( foundTables.contains ( htable )) {

				printf ( DUMP_FORMAT " (see above)", htable, "table", name );
				break;
			}
			else {

				foundTables.insert ( htable );

				printf ( format, htable, "table", name );

				if ( verbose ) {

					printf ( "\n" );
					lua_pushnil ( state );

					while ( lua_next ( state, idx ) ) {

						STLString elementName( name );
						elementName.append ( "." );
						elementName.append ( lua_tostring ( state, -2 ));
						dumpType ( state, -1, elementName.c_str (), verbose, foundTables );
						lua_pop ( state, 1 );
					}
				}
			}
		}
			return; // suppress newline

		case LUA_TTHREAD:

			printf ( format, thvalue( tvalue ), "thread", name );
			break;

		case LUA_TUSERDATA:

			if ( lua_islightuserdata ( state, idx ) ) {
				
				printf ( format, lua_topointer ( state, idx ) , "light userdata", name );
			}
			else {

				printf ( format, lua_topointer( state, idx ), "userdata", name );

				if ( verbose ) {

					lua_getglobal ( state, "tostring" );
					lua_pushvalue ( state, idx );
					
					lua_pcall ( state, 1, 1, 0 );

					printf( "\n\t%s", lua_tostring ( state, -1 ));
					state.Pop ( 1 );
				}
			}
			break;

		default:
			printf ( "*** Unexpected type: %d ***", lua_type ( state, idx ));
	}

	printf ( "\n" );
}

//----------------------------------------------------------------//
static void dumpTypeByAddress ( lua_State* L, TValue* tvalue, const char *name, bool verbose, TableSet& foundTables ) {

	USLuaState state ( L );
	
	lua_lock ( L );
	setobj2s ( L, L->top, tvalue );
	L->top++;
	lua_unlock ( L );

	dumpType ( L, -1, name, verbose, foundTables );
	lua_pop ( L, 1 );
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
	dumpType ( state, 2, name, verbose, foundTables );

	return 0;
}

//----------------------------------------------------------------//
static int _dumpStack ( lua_State* L ) {

	USLuaState state ( L );

	bool verbose = state.GetValue < bool >( 1, true );
	int idx = 0;

	TableSet foundTables;
	for ( TValue* tvalue = state->stack; tvalue < state->top; ++tvalue ) {

		printf ( "stack [ %d ] ", idx++ );
		dumpTypeByAddress ( state, tvalue, "", verbose, foundTables );
	}

	return 0;
}

//----------------------------------------------------------------//
static int _traceback ( lua_State *L ) {

	if ( lua_isstring ( L, 1 )) {  // 'message' a string?
	
		cc8* msg = lua_tostring ( L, 1 );
		printf ( "%s\n", msg );
	}
	
	USLuaState state ( L );
	state.PrintStackTrace ( 1 );
	
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
	this->mMainState = lua_open ();

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
USLuaRuntime::USLuaRuntime () {
}

//----------------------------------------------------------------//
USLuaRuntime::~USLuaRuntime () {

	if ( this->mMainState ) {
		lua_close ( this->mMainState );
	}
	this->mMainState = 0;
}
