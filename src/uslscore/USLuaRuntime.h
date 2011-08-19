// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef LUARUNTIME_H
#define LUARUNTIME_H

#include <uslscore/STLArray.h>
#include <uslscore/STLMap.h>
#include <uslscore/USGlobals.h>
#include <uslscore/USLuaRef.h>
#include <uslscore/USLuaState.h>
#include <uslscore/USLuaStateHandle.h>

//================================================================//
// USLuaRuntime
//================================================================//
class USLuaRuntime :
	public USGlobalClass < USLuaRuntime > {
private:

	typedef STLMap < USLuaObject*, STLString >	LeakMap;
	typedef STLArray < USLuaObject* >			LeakPtrList;
	typedef STLMap < STLString, LeakPtrList >	LeakStackMap;

	bool				mLeakTrackingEnabled;
	LeakMap				mLeaks;

	USLuaRefTable		mWeakRefTable;
	USLuaRefTable		mStrongRefTable;

	USLuaRef			mTraceback;
	USLuaState			mMainState;

	size_t				mTotalBytes;

	//----------------------------------------------------------------//
	static int				_panic					( lua_State *L );
	static void*			_tracking_alloc			( void *ud, void *ptr, size_t osize, size_t nsize );

	//----------------------------------------------------------------//
	void					FindAndPrintLuaRefs		( int idx, cc8* prefix, FILE *f, const LeakPtrList& objects );
	static bool				IsLuaIdentifier			( const char *str );
	void					SetObjectStackTrace		( USLuaObject& object, cc8* trace );

public:

	friend class USLuaObject;
	friend class USLuaRef;
	friend class USLuaState;

	//----------------------------------------------------------------//
	void					Close					();
	void					EnableLeakTracking		( bool enable );
	void					ForceGarbageCollection	();
	size_t					GetMemoryUsage			();
	bool					IsOpen					();
	void					LoadLibs				( cc8* runtimeLibName );
	USLuaStateHandle		Open					();
	void					RegisterModule			( cc8* name, lua_CFunction loader, bool autoLoad );
	void					ReportLeaksFormatted	( FILE *f );
	void					ReportLeaksRaw			( FILE *f );
	void					ReportLuaRefs			( FILE *f );
	void					ResetLeakTracking		();
	void					SetPath					( cc8* path );
	USLuaStateHandle		State					();
							USLuaRuntime			();
							~USLuaRuntime			();
};

#endif
