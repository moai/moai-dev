// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOROUTINE_H
#define	MOAICOROUTINE_H

#include <moai-sim/MOAIAction.h>

class MOAICoroutine;

//================================================================//
// MOAICoroutine
//================================================================//
/**	@lua MOAICoroutine
	@text Binds a Lua coroutine to a MOAIAction.
*/
class MOAICoroutine :
	public MOAIAction {
private:

	MOAILuaMemberRef	mRef; // reference to the coroutine
	lua_State*			mState;
	u32					mNarg;
	STLString			mFuncName;
	STLString			mTrackingGroup;
	
	bool				mIsUpdating;
	bool				mIsActive;
	bool				mIsDefaultParent;
	
	//----------------------------------------------------------------//
	static int			_blockOnAction			( lua_State* L );
	static int			_currentThread			( lua_State* L );
	static int			_getHistogram			( lua_State* L );
	static int			_getTrackingGroup		( lua_State* L );
	static int			_reportHistogram		( lua_State* L );
	static int			_reportLeaks			( lua_State* L );
	static int			_run					( lua_State* L );
	static int			_setDefaultParent		( lua_State* L );
	static int			_setTrackingGroup		( lua_State* L );
	static int			_step					( lua_State* L );
	
	//----------------------------------------------------------------//
	void				OnStart					();
	void				OnStop					();
	int					Resume					( float step );

protected:
	STLString			GetDebugInfo			() const;

public:
	
	DECL_LUA_FACTORY ( MOAICoroutine )
	
	//----------------------------------------------------------------//
	MOAIAction*			GetDefaultParent		();
	bool				IsDone					();
						MOAICoroutine			();
						~MOAICoroutine			();
	void				OnUpdate				( double step );
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
