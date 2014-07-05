// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-core/MOAILogMgr.h>
#include <moai-core/MOAILuaObject.h>
#include <moai-core/MOAILuaState.h>
#include <moai-core/MOAIScopedLuaState.h>
#include <moai-core/MOAILuaRuntime.h>
#include <moai-core/MOAILuaRef.h>

#include <moai-core/MOAILuaState-impl.h>
#include <moai-core/MOAILuaClass-impl.h>

#if !MOAI_WITH_LUAJIT
	#include <lstate.h>
#endif

#define DUMP_FORMAT "%p <%s> %s"

typedef STLSet < struct Table* > TableSet;

//================================================================//
// local
//================================================================//

#if LUA_VERSION_NUM < 502

	//----------------------------------------------------------------//
	// TODO: move into MOAILuaState
	static void lua_pushglobaltable ( lua_State* l ) {
		lua_pushvalue ( l, LUA_GLOBALSINDEX );
	}

#endif

#if ( LUA_VERSION_NUM < 502 ) && ( !MOAI_WITH_LUAJIT )

	//----------------------------------------------------------------//
	static void _dumpType ( lua_State* L, int idx, const char *name, bool verbose, TableSet& foundTables ) {

		MOAILuaState state ( L );

		const char *format = DUMP_FORMAT;

		idx = state.AbsIndex( idx );
		StkId tvalue = state->base + idx - 1;

		switch ( lua_type ( state, idx )) {

			case LUA_TBOOLEAN:

				ZLLog::LogF ( ZLLog::CONSOLE, format, tvalue, "bool", name );
				ZLLog::LogF ( ZLLog::CONSOLE, " = %s", lua_toboolean ( state, idx ) ? "true" : "false" );
				break;

			case LUA_TFUNCTION: {

				const char *funcType = iscfunction ( tvalue ) ? "C function" : "Lua function";

				ZLLog::LogF ( ZLLog::CONSOLE, format, clvalue ( tvalue ), funcType, name );
				break;
			}

			case LUA_TLIGHTUSERDATA:

				ZLLog::LogF ( ZLLog::CONSOLE, format, pvalue ( tvalue ), "pointer", name );
				break;

			case LUA_TNIL:

				ZLLog::LogF ( ZLLog::CONSOLE, format, tvalue, "nil", name );
				break;

			case LUA_TNONE:
				 // Intentionally do nothing--not even the line break.
				return;

			case LUA_TNUMBER:

				ZLLog::LogF ( ZLLog::CONSOLE, format, tvalue, "number", name );
				ZLLog::LogF ( ZLLog::CONSOLE, " = %f", lua_tonumber ( state, idx ));
				break;

			case LUA_TSTRING:

				ZLLog::LogF ( ZLLog::CONSOLE, format, rawtsvalue( tvalue ), "string", name );
				ZLLog::LogF ( ZLLog::CONSOLE, " = \"%s\"", lua_tostring ( state, idx ));
				break;

			case LUA_TTABLE: {

				struct Table* htable = hvalue( tvalue );

				if ( foundTables.contains ( htable )) {

					ZLLog::LogF ( ZLLog::CONSOLE, DUMP_FORMAT " (see above)", htable, "table", name );
					break;
				}
				else {

					foundTables.insert ( htable );

					ZLLog::LogF ( ZLLog::CONSOLE, format, htable, "table", name );

					if ( verbose ) {

						ZLLog::LogF ( ZLLog::CONSOLE, "\n" );
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

				ZLLog::LogF ( ZLLog::CONSOLE, format, thvalue( tvalue ), "thread", name );
				break;

			case LUA_TUSERDATA:

				if ( lua_islightuserdata ( state, idx ) ) {
					
					ZLLog::LogF ( ZLLog::CONSOLE, format, lua_topointer ( state, idx ) , "light userdata", name );
				}
				else {

					ZLLog::LogF ( ZLLog::CONSOLE, format, lua_topointer( state, idx ), "userdata", name );

					if ( verbose ) {

						lua_getglobal ( state, "tostring" );
						lua_pushvalue ( state, idx );
						
						lua_pcall ( state, 1, 1, 0 );

						ZLLog::LogF ( ZLLog::CONSOLE, "\n\t%s", lua_tostring ( state, -1 ));
						state.Pop ( 1 );
					}
				}
				break;

			default:
				ZLLog::LogF ( ZLLog::CONSOLE, "*** Unexpected type: %d ***", lua_type ( state, idx ));
		}

		ZLLog::LogF ( ZLLog::CONSOLE, "\n" );
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

#endif

//================================================================//
// MOAILuaRuntime Lua API
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAILuaRuntime::_clearRef ( lua_State* L ) {
	MOAILuaState state ( L );

	int ref = state.GetValue < int >( 2, false );
	MOAILuaRuntime::Get ().ClearRef ( ref );

	return 0;
}

//----------------------------------------------------------------//
int MOAILuaRuntime::_debugCall ( lua_State* L ) {

	MOAILuaState state ( L );

	state.DebugCall ( 0, 0 );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAILuaRuntime::_deref ( lua_State* L ) {
	MOAILuaState state ( L );

	int ref = state.GetValue < int >( 2, false );
	MOAILuaRuntime::Get ().PushRef ( state, ref );

	return 1;
}

//----------------------------------------------------------------//
int MOAILuaRuntime::_dump ( lua_State* L ) {
	UNUSED ( L );

	#if ( LUA_VERSION_NUM < 502 ) && ( !MOAI_WITH_LUAJIT )

		MOAILuaState state ( L );

		if ( !state.CheckParams ( 1, "S." )) return 0;

		cc8* name = state.GetValue < cc8* >( 1, "" );
		bool verbose = state.GetValue < bool >( 3, true );

		TableSet foundTables;
		_dumpType ( state, 2, name, verbose, foundTables );

	#endif

	return 0;
}

//----------------------------------------------------------------//
int MOAILuaRuntime::_dumpStack ( lua_State* L ) {
	UNUSED ( L );

	#if ( LUA_VERSION_NUM < 502 ) && ( !MOAI_WITH_LUAJIT )

		MOAILuaState state ( L );

		bool verbose = state.GetValue < bool >( 1, true );
		int idx = 0;

		TableSet foundTables;
		for ( TValue* tvalue = state->stack; tvalue < state->top; ++tvalue ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "stack [ %d ] ", idx++ );
			_dumpTypeByAddress ( state, tvalue, "", verbose, foundTables );
		}
	#endif
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getHistogram
	@text	Generates a histogram of active MOAIObjects and returns it
			in a table containing object tallies indexed by object
			class names.

	@opt	string trackingGroup
	@out	table histogram
*/
int MOAILuaRuntime::_getHistogram ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* trackingGroup = state.GetValue < cc8* >( 1, 0 );
	MOAILuaRuntime::Get ().PushHistogram ( state, trackingGroup );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAILuaRuntime::_getRef ( lua_State* L ) {
	MOAILuaState state ( L );

	bool weak = state.GetValue < bool >( 2, false );
	state.Push ( MOAILuaRuntime::Get ().GetRef ( state, 1, weak ));

	return 1;
}

//----------------------------------------------------------------//
int MOAILuaRuntime::_panic ( lua_State *L ) {

	MOAILuaState state ( L );
	state.PrintStackTrace ( ZLLog::CONSOLE, NULL, 1 );
	ZLLog::LogF ( ZLLog::CONSOLE, "PANIC: unprotected error in call to Lua API (%s)\n", lua_tostring ( L, -1 ));
	
	return 0;
}

//----------------------------------------------------------------//
int MOAILuaRuntime::_reportGC ( lua_State* L ) {

	MOAILuaState state ( L );
	MOAILuaRuntime::Get ().mReportGC = state.GetValue < bool >( -1, false );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	reportHistogram
	@text	Generates a histogram of active MOAIObjects.

	@opt	string filename
	@opt	string trackingGroup
	@out	nil
*/
int MOAILuaRuntime::_reportHistogram ( lua_State* L ) {
	MOAILuaState state ( L );

	cc8* filename			= state.GetValue < cc8* >( 1, 0 );
	cc8* trackingGroup		= state.GetValue < cc8* >( 2, 0 );
	
	MOAILuaRuntime::Get ().ReportHistogram ( filename, trackingGroup );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	reportLeaks
	@text	Analyze the currently allocated MOAI objects and create a textual
			report of where they were declared, and what Lua references (if any)
			can be found. NOTE: This is incredibly slow, so only use to debug
			leaking memory issues.
 
			This will also trigger a full garbage collection before performing
			the required report. (Equivalent of collectgarbage("collect").)
 
	@opt	string filename
	@opt	string trackingGroup
	@out	nil
*/
int MOAILuaRuntime::_reportLeaks ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* filename			= state.GetValue < cc8* >( 1, 0 );
	cc8* trackingGroup		= state.GetValue < cc8* >( 2, 0 );
	
	MOAILuaRuntime::Get ().ReportLeaksFormatted ( filename, trackingGroup );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTrackingFlags
	@text	Enable extra memory book-keeping measures that allow all MOAI objects to be
			tracked back to their point of allocation (in Lua). Use together with
			MOAILuaRuntime.reportLeaks() to determine exactly where your memory usage is
			being created. NOTE: This is expensive in terms of both CPU and
			the extra memory associated with the stack info book-keeping. Use only
			when tracking down leaks.
 
	@opt	number flags		Bitwise combination of TRACK_OBJECTS and TRACK_OBJECTS_STACK_TRACE. Default value is 0.
	@out	nil
*/
int MOAILuaRuntime::_setTrackingFlags ( lua_State* L ) {

	MOAILuaState state ( L );
	MOAILuaRuntime::Get ().SetTrackingFlags ( state.GetValue < u32 >( -1, 0 ));
	return 0;
}

//----------------------------------------------------------------//
int MOAILuaRuntime::_traceback ( lua_State *L ) {
	
	MOAILuaRuntime& runtime = MOAILuaRuntime::Get ();
	MOAILuaState state ( L );
	
	cc8* msg = NULL;
	if ( lua_isstring ( L, 1 )) {  // 'message' a string?
		msg = lua_tostring ( L, 1 );
	}
	
	if ( runtime.mTracebackFunc ) {
		runtime.mTracebackFunc ( msg, L, 0 );
	}
	
	if ( runtime.mTracebackRef ) {
		
		if ( runtime.mTracebackRef.PushRef ( state )) {
			
			lua_pushvalue ( state, 1 );
			int result = lua_pcall ( state, 1, 0, 2 );
			if ( result ) {
				ZLLog::LogF ( ZLLog::CONSOLE, "error in user supplied traceback func\n" );
				ZLLog::LogF ( ZLLog::CONSOLE, "falling back on default error handler:\n" );
			}
		}
	}

	if ( msg ) {
		ZLLog::LogF ( ZLLog::CONSOLE, "%s\n", msg );
	}
	state.PrintStackTrace ( ZLLog::CONSOLE, NULL, 0 );

	return 0;
}

//----------------------------------------------------------------//
void* MOAILuaRuntime::_trackingAlloc ( void *ud, void *ptr, size_t osize, size_t nsize ) {
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
// MOAILuaRuntime
//================================================================//

//----------------------------------------------------------------//
void MOAILuaRuntime::BuildHistogram ( HistMap& histogram, cc8* trackingGroup ) {

	TrackingMapIt trackingMapIt = this->mTrackingMap.begin ();
	for ( ; trackingMapIt != this->mTrackingMap.end (); ++trackingMapIt ) {
	
		MOAILuaObjectInfo& info = trackingMapIt->second;
		if ( trackingGroup && ( info.mTrackingGroup.compare ( trackingGroup ) != 0 )) continue;
	
		MOAILuaObject* obj = trackingMapIt->first;
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
void MOAILuaRuntime::ClearRef ( int ref ) {

	if ( ref != LUA_NOREF ) {
		if ( ref & WEAK_REF_BIT ) {
			this->mWeakRefs.Unref ( ref & REF_MASK );
		}
		else {
			this->mStrongRefs.Unref ( ref & REF_MASK );
		}
	}
}

//----------------------------------------------------------------//
void MOAILuaRuntime::Close () {

	if ( this->mState ) {
		lua_close ( this->mState );
		this->mState = 0;
	}
}

//----------------------------------------------------------------//
void MOAILuaRuntime::DeregisterObject ( MOAILuaObject& object ) {

	if ( this != &object ) {
		this->mObjectCount--;
		this->mTrackingMap.erase ( &object );
	}
}

//----------------------------------------------------------------//
// This monstrosity will walk through all tables and functions accessible in the
// current lua state and update the reference line for each one found to help
// track who is pointing to it.
void MOAILuaRuntime::FindLuaRefs ( lua_State* L, int idx, FILE* file, STLString path, cc8* trackingGroup, ObjectPathMap& pathMap, TraversalSet& traversalSet ) {

	L = L ? L : ( lua_State* )this->mState;
	MOAILuaState state ( L );
	idx = state.AbsIndex ( idx );
	
	int type = lua_type ( state, idx );
	
	if ( type == LUA_TUSERDATA ) {
		MOAILuaObject* object = MOAILuaObject::IsMoaiUserdata ( state, idx ) ? ( MOAILuaObject* )state.GetPtrUserData ( idx ) : NULL;
		if ( object && this->mTrackingMap.contains ( object )) {
			MOAILuaObjectInfo& objectInfo = this->mTrackingMap [ object ];
			if ( !trackingGroup || ( objectInfo.mTrackingGroup.compare ( trackingGroup ) != 0 )) {
				StringSet& pathSet = pathMap [ object ];
				pathSet.affirm ( path );
			}
			
		}
	}
	
	// bail if we've already iterated into this item
	const void* addr = lua_topointer ( state, idx );
	if ( traversalSet.contains ( addr )) return;
	traversalSet.insert ( addr );
	
	//printf ( "%s\n", path.c_str ());
	
	switch ( type ) {
	
		// how to deal with weak tables?
		case LUA_TTABLE: {
			
			u32 itr = state.PushTableItr ( idx );
			while ( state.TableItrNext ( itr )) {
				
				STLString keyPath = path;
				
				int keyIdx = state.AbsIndex ( -2 );
				int valIdx = state.AbsIndex ( -1 );
				
				int keyType = lua_type ( state, keyIdx );
				
				cc8* keyName = lua_tostring ( state, keyIdx );
				
				// update the path and follow the keys (if iterable)
				switch ( keyType ) {
					
					case LUA_TBOOLEAN:
					case LUA_TNUMBER:
						keyPath.write ( "[%s]", keyName );
						break;
					
					case LUA_TSTRING:
						if ( MOAILuaRuntime::IsLuaIdentifier ( keyName )) {
							keyPath.write ( ".%s", keyName );
						}
						else {
							keyPath.write ( "[\'%s\']", keyName );
						}
						break;
					
					default: {
						
						STLString keyTypeName;
						const void* keyAddr = lua_topointer ( state, keyIdx );
					
						switch ( keyType ) {
							case LUA_TFUNCTION:
								keyTypeName = "function";
								break;
							
							case LUA_TTABLE:
								keyTypeName = "table";
								break;
								
							case LUA_TUSERDATA:
								
								if ( MOAILuaObject::IsMoaiUserdata ( state, keyIdx )) {
									MOAILuaObject* object = state.GetLuaObject < MOAILuaObject >( keyIdx, false );
									keyTypeName = object->TypeName ();
									keyAddr = object;
								}
								else {
									keyTypeName = "userdata";
								}
								break;
						}
						
						keyPath.write ( "[%s: %p]", keyTypeName.c_str (), keyAddr );
						this->FindLuaRefs ( state, keyIdx, file, STLString::build ( "%s.<key>", keyPath.c_str ()), trackingGroup, pathMap, traversalSet );
					}
				}
				
				// follow the values
				this->FindLuaRefs ( state, valIdx, file, keyPath, trackingGroup, pathMap, traversalSet );
			}
			break;
		}
		case LUA_TFUNCTION: {
			
			cc8* upname;
			for ( int i = 1; ( upname = lua_getupvalue ( state, idx, i )) != NULL; ++i ) {
				STLString upPath = STLString::build ( "%s.<upvalue: ", path.c_str ());
				
				if ( upname [ 0 ]) {
					upPath.write ( "%s>", upname );
				}
				else {
					upPath.write ( "%d>", i );
				}
				
				this->FindLuaRefs ( state, -1, file, upPath.c_str (), trackingGroup, pathMap, traversalSet );
				state.Pop ( 1 );
			}
			
			lua_getfenv ( state, idx );
			this->FindLuaRefs ( state, -1, file, STLString::build ( "%s.<env: %p>", path.c_str (), lua_topointer ( state, -1 )), trackingGroup, pathMap, traversalSet );
			state.Pop ( 1 );
			break;
		}
		case LUA_TTHREAD: {
		
			STLString funcPath = STLString::build ( "%s.<func>", path.c_str ());
		
			MOAILuaState threadState ( lua_tothread ( state, idx ));
		
			lua_Debug ar;
			lua_getstack ( threadState, 1, &ar );
			
			cc8* localname;
			for ( int i = 1; ( localname = lua_getlocal ( threadState, &ar, i )) != NULL; ++i ) {
			
				STLString localPath = STLString::build ( "%s.<local: %s>", funcPath.c_str (), localname );
				this->FindLuaRefs ( threadState, -1, file, localPath.c_str (), trackingGroup, pathMap, traversalSet );
				threadState.Pop ( 1 );
			}
		
			lua_getinfo ( threadState, "f", &ar );
			this->FindLuaRefs ( threadState, -1, file, funcPath.c_str (), trackingGroup, pathMap, traversalSet );
			threadState.Pop ( 1 );
			
			break;
			
		}
	}
	
	// follow the metatable
	if (( type == LUA_TTABLE ) || ( type == LUA_TUSERDATA )) {
		if ( lua_getmetatable ( state, -1 )) {
			path.write ( ".<meta: 0x%p>", lua_topointer ( state, -1 ));
			this->FindLuaRefs ( state, -1, file, path, trackingGroup, pathMap, traversalSet );
			state.Pop ( 1 );
		}
	}
}

//----------------------------------------------------------------//
void MOAILuaRuntime::ForceGarbageCollection () {

	lua_State* L = this->mState;

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
MOAILuaState& MOAILuaRuntime::GetMainState () {
	return this->mState;
}

//----------------------------------------------------------------//
int MOAILuaRuntime::GetRef ( MOAILuaState& state, int idx, bool isWeak ) {
	
	if ( lua_isnil ( state, idx )) return LUA_NOREF;
	return isWeak ? ( this->mWeakRefs.Ref ( state, idx ) | WEAK_REF_BIT ) : this->mStrongRefs.Ref ( state, idx );
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

	return ( this->mState != 0 );
}

//----------------------------------------------------------------//
void MOAILuaRuntime::LoadLibs () {

	// Load the standard Lua libs
	luaL_openlibs ( this->mState );
}

//----------------------------------------------------------------//
MOAILuaRuntime::MOAILuaRuntime () :
	mTrackingFlags ( false ),
	mTracebackFunc ( 0 ),
	mTotalBytes ( 0 ),
	mObjectCount ( 0 ),
	mAllocLogEnabled ( false ),
	mReportGC ( false ) {
}

//----------------------------------------------------------------//
MOAILuaRuntime::~MOAILuaRuntime () {
}

//----------------------------------------------------------------//
void MOAILuaRuntime::OnGlobalsFinalize () {
	this->Close ();
}

//----------------------------------------------------------------//
void MOAILuaRuntime::OnGlobalsRestore () {
}

//----------------------------------------------------------------//
void MOAILuaRuntime::OnGlobalsRetire () {
}

//----------------------------------------------------------------//
MOAIScopedLuaState MOAILuaRuntime::Open () {

	if ( this->IsOpen ()) {
		this->Close ();
	}

	// open the main state
	#if (MOAI_WITH_LUAJIT && (defined(__x86_64 ) || defined(__amd64)) )
		this->mState = luaL_newstate();  //luajit doesn't support custom allocs on 64bit
	#else
		this->mState = lua_newstate ( _trackingAlloc, NULL );
	#endif
	
	lua_atpanic ( this->mState, &_panic );

	// set up the ref tables
	this->mStrongRefs.InitStrong ();
	this->mWeakRefs.InitWeak ();
	
	return MOAIScopedLuaState ( this->mState );
}

//----------------------------------------------------------------//
void MOAILuaRuntime::PushHistogram ( MOAILuaState& state, cc8* trackingGroup  ) {
	
	lua_newtable ( state );
	
	HistMap histogram;
	this->BuildHistogram ( histogram, trackingGroup );
	
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
bool MOAILuaRuntime::PushRef ( MOAILuaState& state, int ref ) {

	if ( ref == LUA_NOREF ) {
		lua_pushnil ( state );
		return false;
	}

	if ( ref & WEAK_REF_BIT ) {
		this->mWeakRefs.PushRef ( state, ref & REF_MASK );
	}
	else {
		this->mStrongRefs.PushRef ( state, ref & REF_MASK );
	}
	return !lua_isnil ( state, -1 );
}

//----------------------------------------------------------------//
void MOAILuaRuntime::PushTraceback ( MOAILuaState& state ) {
	
	state.Push ( _traceback );
}

//----------------------------------------------------------------//
void MOAILuaRuntime::PrintTracking ( MOAILuaObject& object ) {

	if ( this->mTrackingMap.contains ( &object )) {
	
		printf ( "Object <%p> created at:\n", &object );
		printf ( "%s", this->mTrackingMap [ &object ].mStackTrace.c_str ());
	}
}

//----------------------------------------------------------------//
void MOAILuaRuntime::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "TRACK_OBJECTS",					( u32 )TRACK_OBJECTS );
	state.SetField ( -1, "TRACK_OBJECTS_STACK_TRACE",		( u32 )TRACK_OBJECTS_STACK_TRACE );

	luaL_Reg regTable [] = {
		{ "clearRef",				_clearRef },
		{ "debugCall",				_debugCall },
		{ "deref",					_deref },
		{ "dump",					_dump },
		{ "dumpStack",				_dumpStack },
		{ "getHistogram",			_getHistogram },
		{ "getRef",					_getRef },
		{ "reportGC",				_reportGC },
		{ "reportHistogram",		_reportHistogram },
		{ "reportLeaks",			_reportLeaks },
		{ "setTrackingFlags",		_setTrackingFlags },
		{ "traceback",				_traceback },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAILuaRuntime::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAILuaRuntime::RegisterModule ( cc8* name, lua_CFunction loader, bool autoLoad ) {

	this->mState.RegisterModule ( loader, name, autoLoad );
}

//----------------------------------------------------------------//
void MOAILuaRuntime::RegisterObject ( MOAILuaObject& object ) {

	if ( this != &object ) {

		this->mObjectCount++;
		
		if ( this->mTrackingFlags & TRACK_OBJECTS ) {
		
			MOAILuaObjectInfo& info = this->mTrackingMap [ &object ];
			info.mTrackingGroup = this->mTrackingGroup;
		}
	}
}

//----------------------------------------------------------------//
void MOAILuaRuntime::RegisterObject ( MOAILuaState& state, MOAILuaObject& object ) {

	if ( this != &object ) {
	
		if ( this->mTrackingFlags & TRACK_OBJECTS_STACK_TRACE ) {
		
			MOAILuaObjectInfo& info = this->mTrackingMap [ &object ];
			info.mStackTrace = state.GetStackTrace ( "object allocation:", 1 );
		}
	}
}

//----------------------------------------------------------------//
void MOAILuaRuntime::ReportHistogram ( cc8* filename, cc8* trackingGroup ) {
	
	FILE* file	= MOAILogMgr::Get ().GetFile ();
	FILE* log	= 0;
	
	if ( filename ) {
		log = fopen ( filename, "w" );
		file = log;
		assert ( log );
	}
	
	size_t totalTracked = this->mTrackingMap.size ();
	ZLLog::LogF ( file, "tracking %d of %d allocated MOAIObjects\n", ( int )totalTracked, ( int )this->mObjectCount );
	
	HistMap histogram;
	this->BuildHistogram ( histogram, trackingGroup );
	
	HistMap::iterator histogramIt = histogram.begin ();
	for ( ; histogramIt != histogram.end (); ++histogramIt ) {
	
		const STLString& name = histogramIt->first;
		size_t count = histogramIt->second;
		float percent = (( float )count / ( float )totalTracked ) * 100.0f;
	
		ZLLog::LogF ( file, "%-32.32s %d (%.2f%% of %d)\n", name.str (), ( int )count, percent, ( int )totalTracked );
	}
	
	if ( log ) {
		fclose ( log );
	}
}

//----------------------------------------------------------------//
void MOAILuaRuntime::ReportLeaksFormatted ( cc8* filename, cc8* trackingGroup ) {

	FILE* file	= MOAILogMgr::Get ().GetFile ();
	FILE* log	= 0;
	
	if ( filename ) {
		log = fopen ( filename, "w" );
		file = log;
		assert ( log );
	}

	MOAILuaState& state = this->mState;
	
	// Update Lua references for all tracked objects
	
	TraversalSet traversalSet; // to hold traversed objects
	ObjectPathMap pathMap; // to hold sets of Lua reference paths to objects
	
	// check all the globals
	lua_pushglobaltable ( state );
	this->FindLuaRefs ( state, -1, file, "_G", trackingGroup, pathMap, traversalSet );
	state.Pop ( 1 );
	
	// print the strong refs
	this->mStrongRefs.PushRefTable ( state );
	this->FindLuaRefs ( state, -1, file, "<STRONG REFS>", trackingGroup, pathMap, traversalSet );
	state.Pop ( 1 );
	
	// Correlate leaks by identical stack traces.
	LeakStackMap stacks;
	
	for ( TrackingMapConstIt i = this->mTrackingMap.begin (); i != this->mTrackingMap.end (); ++i ) {
		const MOAILuaObjectInfo& info = i->second;
		if ( trackingGroup && ( info.mTrackingGroup.compare ( trackingGroup ) != 0 )) continue;
		
		ObjectSet& objectSet = stacks [ info.mStackTrace ];
		objectSet.affirm ( i->first );
	}
	
	ZLLog::LogF ( file, "------------------------------------------------\n" );
	ZLLog::LogF ( file, "-- BEGIN LUA OBJECT LEAKS --\n" );
	
	// Then, print out each unique allocation spot along with all references
	// (including multiple references) followed by the alloction stack
	int top = state.GetTop ();
	UNUSED ( top );
	
	for ( LeakStackMapIt i = stacks.begin (); i != stacks.end (); ++i ) {
		
		const ObjectSet& objectSet = i->second;
		ZLLog::LogF ( file, "Allocation: %lu\n", objectSet.size ());
		
		for ( ObjectSetIt objectSetIt = objectSet.begin (); objectSetIt != objectSet.end (); ++objectSetIt ) {
			MOAILuaObject* object = *objectSetIt;
			ZLLog::LogF ( file, "<%s> %p\n", object->TypeName (), object );
			
			if ( pathMap.contains ( object )) {
				StringSet& pathSet = pathMap [ object ];
				for ( StringSetIt j = pathSet.begin (); j != pathSet.end (); ++j ) {
					ZLLog::LogF ( file, "path: %s\n", j->c_str ());
				}
			}
		}
		
		// print the stack trace
		ZLLog::LogF ( file, i->first.c_str ());
		ZLLog::LogF ( file, "\n" );
	}
	
	assert ( top == state.GetTop ());
	
	ZLLog::LogF ( file, "-- END LUA LEAKS --\n" );
	
	if ( log ) {
		fclose ( log );
	}
}

//----------------------------------------------------------------//
void MOAILuaRuntime::ReportLeaksRaw ( cc8* filename, cc8* trackingGroup ) {

	FILE* file	= MOAILogMgr::Get ().GetFile ();
	FILE* log	= 0;
	
	if ( filename ) {
		log = fopen ( filename, "w" );
		file = log;
		assert ( log );
	}

	this->ForceGarbageCollection ();
	
	ZLLog::LogF ( file, "-- LUA OBJECT LEAK REPORT ------------\n" );
	u32 count = 0;
	
	for ( TrackingMap::const_iterator i = this->mTrackingMap.begin () ; i != this->mTrackingMap.end (); ++i ) {
		const MOAILuaObjectInfo& info = i->second;
		if ( trackingGroup && ( info.mTrackingGroup.compare ( trackingGroup ) != 0 )) continue;
		ZLLog::LogF ( file, info.mStackTrace.c_str ());
		count++;
	}
	ZLLog::LogF ( file, "-- END LEAK REPORT (Total Objects: %d) ---------\n", count );
	
	if ( log ) {
		fclose ( log );
	}
}

//----------------------------------------------------------------//
void MOAILuaRuntime::ResetTracking () {

	this->mTrackingMap.clear ();
}

//----------------------------------------------------------------//
void MOAILuaRuntime::SetPath ( cc8* path ) {

	int top = lua_gettop ( this->mState );

	lua_getglobal ( this->mState, "package" );
	int packageIdx = lua_gettop ( this->mState );

	lua_pushstring ( this->mState, "path" );
	lua_pushstring ( this->mState, path );
	lua_settable ( this->mState, packageIdx );

	lua_settop ( this->mState, top );
}

//----------------------------------------------------------------//
void MOAILuaRuntime::SetTrackingFlags ( u32 flags ) {

	this->mTrackingFlags = flags;
	
	if ( !flags ) {
		this->ResetTracking ();
	}
}

//----------------------------------------------------------------//
void MOAILuaRuntime::SetTrackingGroup () {

	this->mTrackingGroup.clear ();
}

//----------------------------------------------------------------//
void MOAILuaRuntime::SetTrackingGroup ( const STLString& trackingGroup ) {

	this->mTrackingGroup = trackingGroup;
}

//----------------------------------------------------------------//
MOAIScopedLuaState MOAILuaRuntime::State () {

	return MOAIScopedLuaState ( this->mState );
}

