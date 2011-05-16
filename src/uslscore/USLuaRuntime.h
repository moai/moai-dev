// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef LUARUNTIME_H
#define LUARUNTIME_H

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

	USLuaRefTable		mWeakRefTable;
	USLuaRefTable		mStrongRefTable;

	USLuaRef			mTraceback;
	USLuaState			mMainState;

	//----------------------------------------------------------------//

public:

	friend class USLuaRef;
	friend class USLuaState;

	//----------------------------------------------------------------//
	void					Close					();
	bool					IsOpen					();
	void					LoadLibs				( cc8* runtimeLibName );
	USLuaStateHandle		Open					();
	void					RegisterModule			( cc8* name, lua_CFunction loader, bool autoLoad );
	void					SetPath					( cc8* path );
	USLuaStateHandle		State					();
							USLuaRuntime			();
							~USLuaRuntime			();
};

#endif
