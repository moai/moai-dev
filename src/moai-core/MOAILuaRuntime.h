// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAILUARUNTIME_H
#define MOAILUARUNTIME_H

#include <moai-core/MOAIEventSource.h>
#include <moai-core/MOAIGlobals.h>

#include <moai-core/MOAILuaObject.h>
#include <moai-core/MOAILuaRef.h>
#include <moai-core/MOAILuaState.h>
#include <moai-core/MOAIScopedLuaState.h>

class MOAILuaObject;

//================================================================//
// MOAILuaRuntime
//================================================================//
class MOAILuaRuntime :
	public MOAIGlobalClass < MOAILuaRuntime, MOAILuaObject > {
public:

	typedef void ( *TracebackFunc ) ( const char* message, struct lua_State* L, int level );

private:

	typedef STLMap < MOAILuaObject*, STLString >	LeakMap;
	typedef STLArray < MOAILuaObject* >				LeakPtrList;
	typedef STLMap < STLString, LeakPtrList >		LeakStackMap;

	typedef STLMap < STLString, size_t >			HistMap;
	typedef STLSet < MOAILuaObject* >				HistSet;

	bool				mHistogramEnabled;
	HistSet				mHistSet;

	bool				mLeakTrackingEnabled;
	LeakMap				mLeaks;

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
	static int				_debugCall				( lua_State* L );
	static int				_dump					( lua_State* L );
	static int				_dumpStack				( lua_State* L );
	static int				_panic					( lua_State *L );
	static int				_reportGC				( lua_State* L );
	static void*			_trackingAlloc			( void *ud, void *ptr, size_t osize, size_t nsize );
	static int				_traceback				( lua_State *L );

	//----------------------------------------------------------------//
	void					BuildHistogram			( HistMap& histogram );
	void					DeregisterObject		( MOAILuaObject& object );
	void					FindAndPrintLuaRefs		( int idx, cc8* prefix, FILE *f, const LeakPtrList& objects );
	static bool				IsLuaIdentifier			( const char *str );
	void					OnGlobalsFinalize		();
	void					OnGlobalsRestore		();
	void					OnGlobalsRetire			();
	void					RegisterObject			( MOAILuaObject& object );

public:

	friend class MOAILuaObject;
	friend class MOAILuaRef;
	friend class MOAILuaStrongRef;
	friend class MOAILuaWeakRef;
	friend class MOAILuaState;

	DECL_LUA_SINGLETON ( MOAILuaRuntime )

	GET ( size_t, ObjectCount, mObjectCount )
	GET ( MOAILuaStrongRef&, TracebackRef, mTracebackRef )
	GET_SET ( bool, AllocLogEnabled, mAllocLogEnabled )
	GET_SET ( TracebackFunc, TracebackFunc, mTracebackFunc )

	//----------------------------------------------------------------//
	void					ClearObjectStackTrace		( MOAILuaObject* object );
	void					Close						();
	void					EnableHistogram				( bool enable );
	void					EnableLeakTracking			( bool enable );
	void					ForceGarbageCollection		();
	size_t					GetMemoryUsage				();
	MOAILuaState&			GetMainState				();
	bool					IsOpen						();
	void					LoadLibs					();
							MOAILuaRuntime				();
							~MOAILuaRuntime				();
	MOAIScopedLuaState		Open						();
	void					PushHistogram				( MOAILuaState& state );
	void					PushTraceback				( MOAILuaState& state );
	void					RegisterLuaClass			( MOAILuaState& state );
	void					RegisterLuaFuncs			( MOAILuaState& state );
	void					RegisterModule				( cc8* name, lua_CFunction loader, bool autoLoad );
	void					ReportHistogram				( FILE *f );
	void					ReportLeaksFormatted		( FILE *f );
	void					ReportLeaksRaw				( FILE *f );
	void					ReportLuaRefs				( FILE *f );
	void					ResetLeakTracking			();
	void					SetObjectStackTrace			( MOAILuaObject* object );
	void					SetPath						( cc8* path );
	MOAIScopedLuaState		State						();							
};

#endif
