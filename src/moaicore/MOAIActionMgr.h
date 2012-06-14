// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIACTIONMGR_H
#define	MOAIACTIONMGR_H

#include <moaicore/MOAIGlobals.h>
#include <moaicore/MOAILua.h>

class MOAIAction;

//================================================================//
// MOAIActionMgr
//================================================================//
/**	@name MOAIActionMgr
	@text Manager class for MOAIActions.
*/
class MOAIActionMgr :
	public MOAIGlobalClass < MOAIActionMgr, MOAILuaObject > {
private:

	static const u32 RESET_PASS	= 0xffffffff;

	u32 mPass;
	u32 mTotalPasses;

	bool mProfilingEnabled;
	bool mThreadInfoEnabled;

	MOAIAction* mRoot;
	MOAIAction* mCurrentAction;

	//----------------------------------------------------------------//
	static int			_getRoot				( lua_State* L );
	static int			_setProfilingEnabled	( lua_State* L );
	static int			_setRoot				( lua_State* L );
	static int			_setThreadInfoEnabled	( lua_State* L );

	//----------------------------------------------------------------//
	MOAIAction*			AffirmRoot				();
	u32					GetNextPass				();

public:

	friend class MOAIAction;

	DECL_LUA_SINGLETON ( MOAIActionMgr )

	GET_SET ( MOAIAction*, CurrentAction, mCurrentAction )
	
	GET_SET ( bool, ProfilingEnabled, mProfilingEnabled )
	GET_SET ( bool, ThreadInfoEnabled, mThreadInfoEnabled )

	//----------------------------------------------------------------//
						MOAIActionMgr			();
						~MOAIActionMgr			();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				Update					( float step );
};

#endif
