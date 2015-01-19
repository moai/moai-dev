// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAILUARUNTIME_H
#define MOAILUARUNTIME_H

//#include <moai-core/MOAIEventSource.h>
#include <moai-core/MOAIGlobals.h>

#include <moai-core/MOAILuaClass.h>
#include <moai-core/MOAILuaObject.h>
#include <moai-core/MOAILuaRef.h>
#include <moai-core/MOAILuaState.h>
#include <moai-core/MOAIScopedLuaState.h>

class MOAILuaObject;

//================================================================//
// MOAILuaObjectInfo
//================================================================//
class MOAILuaObjectInfo {
private:

	friend class MOAILuaRuntime;
	
	STLString	mTrackingGroup;
	STLString	mStackTrace;
};

//================================================================//
// MOAILuaTraversalState
//================================================================//
class MOAILuaTraversalState {
private:

	friend class MOAILuaRuntime;
	
	typedef STLSet < STLString >							StringSet;
	typedef StringSet::iterator								StringSetIt;

	typedef STLSet < const void* >							TraversalSet;
	typedef TraversalSet::iterator							TraversalSetIt;

	typedef STLMap < MOAILuaObject*, StringSet >			ObjectPathMap;
	typedef ObjectPathMap::iterator							ObjectPathMapIt;

	TraversalSet		mTraversalStack; // to hold traversed objects
	TraversalSet		mTraversalSet; // all traversed objects
	TraversalSet		mIgnoreSet; // to hold ignored objects
	ObjectPathMap		mPathMap; // sets of Lua reference paths to objects
	
	bool				mIgnoreTraversed;
};

//================================================================//
// MOAILuaRuntime
//================================================================//
class MOAILuaRuntime :
	public MOAIGlobalClass < MOAILuaRuntime, MOAILuaObject > {
public:

	typedef void ( *TracebackFunc ) ( const char* message, struct lua_State* L, int level );

private:

	static const u32 WEAK_REF_BIT	= 0x80000000;
	static const u32 REF_MASK		= 0x7fffffff;

	typedef STLSet < MOAILuaObject* >						ObjectSet;
	typedef ObjectSet::iterator								ObjectSetIt;

	typedef STLMap < MOAILuaObject*, MOAILuaObjectInfo >	TrackingMap;
	typedef TrackingMap::iterator							TrackingMapIt;
	typedef TrackingMap::const_iterator						TrackingMapConstIt;
	
	typedef STLArray < MOAILuaObject* >						LeakPtrList;
	typedef LeakPtrList::iterator							LeakPtrListIt;
	
	typedef STLMap < STLString, ObjectSet >					LeakStackMap;
	typedef LeakStackMap::iterator							LeakStackMapIt;
	
	typedef STLMap < STLString, size_t >					HistMap;

	STLString			mTrackingGroup;
	u32					mTrackingFlags;
	TrackingMap			mTrackingMap;

	MOAILuaRefTable		mStrongRefs;
	MOAILuaRefTable		mWeakRefs;

	// this is to handle the edge case where an object gets created and bound to Lua, but is not
	// passed back to Lua immediately. the binding's mUserdata is a weak ref, so if additional
	// work is done in Lua, there is a slight chance it may get tagged for garbage collection
	// in an indeterminate way.
	
	// the cache gets thrown out every update by MOAISim.
	MOAILuaStrongRef	mUserdataCache;

	TracebackFunc		mTracebackFunc;			// Custom traceback func
	MOAILuaStrongRef	mTracebackRef;			// Custom traceback ref
	MOAILuaState		mState;

	size_t				mTotalBytes;
	size_t				mObjectCount;			// All MOAIObjects, whether or not bound to Lua

	bool				mAllocLogEnabled;
	bool				mReportGC;

	//----------------------------------------------------------------//
	static int				_clearRef				( lua_State* L );
	static int				_debugCall				( lua_State* L );
	static int				_deref					( lua_State* L );
	static int				_dump					( lua_State* L );
	static int				_dumpStack				( lua_State* L );
	static int				_forceGC				( lua_State* L );
	static int				_getHistogram			( lua_State* L );
	static int				_getRef					( lua_State* L );
	static int				_panic					( lua_State* L );
	static int				_reportGC				( lua_State* L );
	static int				_reportHistogram		( lua_State* L );
	static int				_reportLeaks			( lua_State* L );
	static int				_setTrackingFlags		( lua_State* L );
	static void*			_trackingAlloc			( void *ud, void *ptr, size_t osize, size_t nsize );
	static int				_traceback				( lua_State *L );

	//----------------------------------------------------------------//
	void					BuildHistogram			( HistMap& histogram, cc8* trackingGroup );
	void					DeregisterObject		( MOAILuaObject& object );
	void					FindLuaRefs				( lua_State* L, FILE* file, cc8* trackingGroup, MOAILuaTraversalState& traversalState );
	void					FindLuaRefs				( lua_State* L, FILE* file, STLString path, cc8* trackingGroup, MOAILuaTraversalState& traversalState );
	void					FindLuaRefs				( lua_State* L, int idx, FILE* file, STLString path, cc8* trackingGroup, MOAILuaTraversalState& traversalState );
	int						GetRef					( MOAILuaState& state, int idx, u32 type );
	static bool				IsLuaIdentifier			( const char *str );
	int						MakeStrong				( int refID );
	int						MakeWeak				( int refID );
	void					OnGlobalsFinalize		();
	void					RegisterObject			( MOAILuaObject& object );
	void					RegisterObject			( MOAILuaState& state, MOAILuaObject& object );

public:

	enum {
		TRACK_OBJECTS				= 0x01,
		TRACK_OBJECTS_STACK_TRACE	= 0x02,
	};

	friend class MOAILuaObject;
	friend class MOAILuaRef;
	friend class MOAILuaStrongRef;
	friend class MOAILuaWeakRef;
	friend class MOAILuaState;

	DECL_LUA_SINGLETON ( MOAILuaRuntime )

	GET ( size_t, ObjectCount, mObjectCount )
	GET ( MOAILuaStrongRef&, TracebackRef, mTracebackRef )
	GET ( u32, TrackingFlags, mTrackingFlags )
	
	GET_SET ( bool, AllocLogEnabled, mAllocLogEnabled )
	GET_SET ( TracebackFunc, TracebackFunc, mTracebackFunc )

	//----------------------------------------------------------------//
	void					CacheUserdata				( MOAILuaState& state, int idx );
	void					ClearRef					( int refID );
	void					Close						();
	void					ForceGarbageCollection		();
	size_t					GetMemoryUsage				();
	MOAILuaState&			GetMainState				();
	bool					IsMainThread				( lua_State* L );
	bool					IsOpen						();
	void					LoadLibs					();
							MOAILuaRuntime				();
							~MOAILuaRuntime				();
	MOAIScopedLuaState		Open						();
	void					PurgeUserdata				( MOAILuaState& state, int idx );
	void					PurgeUserdataCache			();
	void					PushHistogram				( MOAILuaState& state, cc8* trackingGroup );
	bool					PushRef						( MOAILuaState& state, int refID );
	void					PushTraceback				( MOAILuaState& state );
	void					PrintTracking				( MOAILuaObject& object );
	void					RegisterLuaClass			( MOAILuaState& state );
	void					RegisterLuaFuncs			( MOAILuaState& state );
	void					RegisterModule				( cc8* name, lua_CFunction loader, bool autoLoad );
	void					ReportHistogram				( cc8* filename, cc8* trackingGroup );
	void					ReportLeaksFormatted		( cc8* filename, cc8* trackingGroup );
	void					ReportLeaksRaw				( cc8* filename, cc8* trackingGroup );
	void					ResetTracking				();
	void					SetPath						( cc8* path );
	void					SetTrackingFlags			( u32 flags );
	void					SetTrackingGroup			();
	void					SetTrackingGroup			( const STLString& trackingGroup );
	MOAIScopedLuaState		State						();	
};

#endif
