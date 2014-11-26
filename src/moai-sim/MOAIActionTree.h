// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIACTIONTREE_H
#define	MOAIACTIONTREE_H

#include <moai-sim/MOAIAction.h>

//================================================================//
// MOAIActionTree
//================================================================//
// TODO: doxygen
class MOAIActionTree :
	public virtual MOAIAction {
private:

	static const u32 RESET_PASS	= 0xffffffff;

	bool mProfilingEnabled;
	bool mThreadInfoEnabled;

	MOAIAction* mRoot;

	//----------------------------------------------------------------//
	MOAIAction*			AffirmRoot				();
	void				OnLostChild				( MOAIAction* child );
	void				OnUpdate				( double step );
	void				SetRoot					( MOAIAction* root );

protected:

	//----------------------------------------------------------------//
	static int			_getRoot				( lua_State* L );
	static int			_setProfilingEnabled	( lua_State* L );
	static int			_setRoot				( lua_State* L );
	static int			_setThreadInfoEnabled	( lua_State* L );

public:

	friend class MOAIAction;

	DECL_LUA_FACTORY ( MOAIActionTree )
	
	GET_SET ( bool, ProfilingEnabled, mProfilingEnabled )
	GET_SET ( bool, ThreadInfoEnabled, mThreadInfoEnabled )

	//----------------------------------------------------------------//
	MOAIAction*			GetDefaultParent		();
	bool				IsDone					();
						MOAIActionTree			();
						~MOAIActionTree			();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				SetDefaultParent		();
	void				SetDefaultParent		( MOAIAction* defaultParent );
	void				Update					( double step );
};

#endif
