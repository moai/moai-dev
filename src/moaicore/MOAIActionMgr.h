// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIACTIONMGR_H
#define	MOAIACTIONMGR_H

class MOAIAction;

//================================================================//
// MOAIActionMgr
//================================================================//
/**	@name MOAIActionMgr
	@text Manager class for MOAIActions.
*/
class MOAIActionMgr :
	public USGlobalClass < MOAIActionMgr, USLuaObject > {
private:

	static const u32 RESET_PASS	= 0xffffffff;

	u32 mPass;
	u32 mTotalPasses;

	USRef < MOAIAction > mRoot;
	MOAIAction* mCurrentAction;

	//----------------------------------------------------------------//
	static int			_getRoot				( lua_State* L );
	static int			_setRoot				( lua_State* L );

	//----------------------------------------------------------------//
	MOAIAction*			AffirmRoot				();
	u32					GetNextPass				();

public:

	friend class MOAIAction;

	DECL_LUA_SINGLETON ( MOAIActionMgr )

	GET_SET ( MOAIAction*, CurrentAction, mCurrentAction )

	//----------------------------------------------------------------//
	void				Clear					();
						MOAIActionMgr			();
						~MOAIActionMgr			();
	void				RegisterLuaClass		( USLuaState& state );
	void				StartAction				( MOAIAction& action );
	void				Update					( float step );
};

#endif
