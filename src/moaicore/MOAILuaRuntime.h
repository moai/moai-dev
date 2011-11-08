// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAILUARUNTIME_H
#define MOAILUARUNTIME_H

#include <moaicore/MOAILuaRef.h>
#include <moaicore/MOAILuaState.h>
#include <moaicore/MOAILuaStateHandle.h>

class MOAIObject;

//================================================================//
// MOAILuaRuntime
//================================================================//
class MOAILuaRuntime :
	public USGlobalClass < MOAILuaRuntime > {
private:

	typedef STLMap < MOAIObject*, STLString >	LeakMap;
	typedef STLArray < MOAIObject* >			LeakPtrList;
	typedef STLMap < STLString, LeakPtrList >	LeakStackMap;

	typedef STLMap < STLString, size_t >		HistMap;
	typedef STLSet < MOAIObject* >				HistSet;

	bool				mHistogramEnabled;
	HistSet				mHistSet;

	bool				mLeakTrackingEnabled;
	LeakMap				mLeaks;

	MOAILuaRefTable		mWeakRefTable;
	MOAILuaRefTable		mStrongRefTable;

	MOAILuaRef			mTraceback;
	MOAILuaState			mMainState;

	size_t				mTotalBytes;
	size_t				mObjectCount;			// All MOAIObjects, whether or not bound to Lua

	bool				mAllocLogEnabled;

	//----------------------------------------------------------------//
	static int				_panic					( lua_State *L );
	static void*			_tracking_alloc			( void *ud, void *ptr, size_t osize, size_t nsize );

	//----------------------------------------------------------------//
	void					DeregisterObject		( MOAIObject& object );
	void					FindAndPrintLuaRefs		( int idx, cc8* prefix, FILE *f, const LeakPtrList& objects );
	static bool				IsLuaIdentifier			( const char *str );
	void					RegisterObject			( MOAIObject& object );

public:

	friend class MOAIObject;
	friend class MOAILuaRef;
	friend class MOAILuaState;

	GET ( size_t, ObjectCount, mObjectCount )
	GET_SET ( bool, AllocLogEnabled, mAllocLogEnabled )

	//----------------------------------------------------------------//
	void					ClearObjectStackTrace		( MOAIObject* object );
	void					Close						();
	void					EnableHistogram				( bool enable );
	void					EnableLeakTracking			( bool enable );
	void					ForceGarbageCollection		();
	size_t					GetMemoryUsage				();
	bool					IsOpen						();
	void					LoadLibs					( cc8* runtimeLibName );
	MOAILuaStateHandle		Open						();
	void					RegisterModule				( cc8* name, lua_CFunction loader, bool autoLoad );
	void					ReportHistogram				( FILE *f );
	void					ReportLeaksFormatted		( FILE *f );
	void					ReportLeaksRaw				( FILE *f );
	void					ReportLuaRefs				( FILE *f );
	void					ResetLeakTracking			();
	void					SetObjectStackTrace			( MOAIObject* object );
	void					SetPath						( cc8* path );
	MOAILuaStateHandle		State						();
							MOAILuaRuntime				();
							~MOAILuaRuntime				();
};

#endif
