// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAILUARUNTIME_H
#define MOAILUARUNTIME_H

#include <moaicore/MOAIGlobals.h>
#include <moaicore/MOAILuaRef.h>
#include <moaicore/MOAILuaState.h>
#include <moaicore/MOAILuaStateHandle.h>

class MOAILuaObject;

//================================================================//
// MOAILuaRuntime
//================================================================//
class MOAILuaRuntime :
	public MOAIGlobalClass < MOAILuaRuntime > {
private:

	typedef STLMap < MOAILuaObject*, STLString >	LeakMap;
	typedef STLArray < MOAILuaObject* >				LeakPtrList;
	typedef STLMap < STLString, LeakPtrList >		LeakStackMap;

	typedef STLMap < STLString, size_t >		HistMap;
	typedef STLSet < MOAILuaObject* >			HistSet;

	bool				mHistogramEnabled;
	HistSet				mHistSet;

	bool				mLeakTrackingEnabled;
	LeakMap				mLeaks;

	MOAILuaRefTable		mWeakRefTable;
	MOAILuaRefTable		mStrongRefTable;

	MOAILuaRef			mTraceback;
	MOAILuaRef			mCustomTraceback;	
	MOAILuaState		mMainState;

	size_t				mTotalBytes;
	size_t				mObjectCount;			// All MOAIObjects, whether or not bound to Lua

	bool				mAllocLogEnabled;

	//----------------------------------------------------------------//
	static int				_panic					( lua_State *L );
	static void*			_tracking_alloc			( void *ud, void *ptr, size_t osize, size_t nsize );

	//----------------------------------------------------------------//
	void					BuildHistogram			( HistMap& histogram );
	void					DeregisterObject		( MOAILuaObject& object );
	void					FindAndPrintLuaRefs		( int idx, cc8* prefix, FILE *f, const LeakPtrList& objects );
	static bool				IsLuaIdentifier			( const char *str );
	void					RegisterObject			( MOAILuaObject& object );

public:

	friend class MOAILuaObject;
	friend class MOAILuaRef;
	friend class MOAILuaState;

	GET ( size_t, ObjectCount, mObjectCount )
	GET_SET ( bool, AllocLogEnabled, mAllocLogEnabled )

	//----------------------------------------------------------------//
	void					ClearObjectStackTrace		( MOAILuaObject* object );
	void					Close						();
	void					EnableHistogram				( bool enable );
	void					EnableLeakTracking			( bool enable );
	void					ForceGarbageCollection		();
	size_t					GetMemoryUsage				();
	MOAILuaState&			GetMainState				();
	MOAILuaRef&				GetCustomTraceback			();
	bool					IsOpen						();
	void					LoadLibs					( cc8* runtimeLibName );
	MOAILuaStateHandle		Open						();
	void					PushHistogram				( MOAILuaState& state );
	void					RegisterModule				( cc8* name, lua_CFunction loader, bool autoLoad );
	void					ReportHistogram				( FILE *f );
	void					ReportLeaksFormatted		( FILE *f );
	void					ReportLeaksRaw				( FILE *f );
	void					ReportLuaRefs				( FILE *f );
	void					ResetLeakTracking			();
	void					SetObjectStackTrace			( MOAILuaObject* object );
	void					SetPath						( cc8* path );
	MOAILuaStateHandle		State						();
							MOAILuaRuntime				();
							~MOAILuaRuntime				();
};

#endif
