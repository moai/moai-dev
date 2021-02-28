// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIACTIONMGR_H
#define	MOAIACTIONMGR_H

#include <moai-core/MOAILua.h>

class MOAIAction;
class MOAIActionTree;

//================================================================//
// MOAIActionMgr
//================================================================//
class MOAIActionMgr :
	public virtual ZLContextClass,
	public MOAILuaObject {
private:
	
	friend class MOAIAction;
	friend class MOAIActionTree;
	friend class MOAICoroutine;
	
	ZLLeanStack < MOAIAction* > 			mStack;
	MOAILuaSharedPtr < MOAIActionTree >		mActionTree;
	
	//----------------------------------------------------------------//
	static int			_getActionTree			( lua_State* L );
	
	//----------------------------------------------------------------//
	MOAIAction*			GetCurrent				();
	MOAIAction*			GetDefaultParent		();
	void				Pop						();
	void				Push					( MOAIAction& action );
	
	//----------------------------------------------------------------//
	void				_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );
	
public:
	
	DECL_LUA_SINGLETON ( MOAIActionMgr )
	
	GET ( MOAIActionTree&, ActionTree, *mActionTree );
	
	//----------------------------------------------------------------//
						MOAIActionMgr		();
						~MOAIActionMgr		();
};

#endif
