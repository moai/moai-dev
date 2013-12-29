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
// MOAILuaRuntime
//================================================================//
class MOAILuaRuntime :
	public MOAIGlobalClass < MOAILuaRuntime, MOAILuaObject > {
public:

	typedef void ( *TracebackFunc ) ( const char* message, struct lua_State* L, int level );

private:

	static const u32 WEAK_REF_BIT	= 0x80000000;
	static const u32 REF_MASK		= 0x7fffffff;

	typedef STLMap < MOAILuaObject*, MOAILuaObjectInfo >	TrackingMap;
	typedef TrackingMap::iterator							TrackingMapIt;
	typedef TrackingMap::const_iterator						TrackingMapConstIt;
	
	typedef STLArray < MOAILuaObject* >						LeakPtrList;
	typedef STLMap < STLString, LeakPtrList >				LeakStackMap;
	typedef STLMap < STLString, size_t >					HistMap;

	STLString			mTrackingGroup;
	u32					mTrackingFlags;
	TrackingMap			mTrackingMap;

	MOAILuaRefTable		mStrongRefs;
	MOAILuaRefTable		mWeakRefs;

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
	static int				_getHistogram			( lua_State* L );
	static int				_getRef					( lua_State* L );
	static int				_panic					( lua_State *L );
	static int				_reportGC				( lua_State* L );
	static int				_reportHistogram		( lua_State* L );
	static int				_reportLeaks			( lua_State* L );
	static int				_setTrackingFlags		( lua_State* L );
	static void*			_trackingAlloc			( void *ud, void *ptr, size_t osize, size_t nsize );
	static int				_traceback				( lua_State *L );

	//----------------------------------------------------------------//
	void					BuildHistogram			( HistMap& histogram, cc8* trackingGroup );
	void					DeregisterObject		( MOAILuaObject& object );
	void					FindAndPrintLuaRefs		( int idx, cc8* prefix, FILE *f, const LeakPtrList& objects );
	static bool				IsLuaIdentifier			( const char *str );
	void					OnGlobalsFinalize		();
	void					OnGlobalsRestore		();
	void					OnGlobalsRetire			();
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
	void					ClearRef					( int ref );
	void					Close						();
	void					ForceGarbageCollection		();
	size_t					GetMemoryUsage				();
	MOAILuaState&			GetMainState				();
	int						GetRef						( MOAILuaState& state, int idx, bool weak );
	bool					IsOpen						();
	void					LoadLibs					();
							MOAILuaRuntime				();
							~MOAILuaRuntime				();
	MOAIScopedLuaState		Open						();
	void					PushHistogram				( MOAILuaState& state, cc8* trackingGroup );
	bool					PushRef						( MOAILuaState& state, int ref );
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
