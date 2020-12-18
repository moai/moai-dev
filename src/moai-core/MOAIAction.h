// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIACTION_H
#define	MOAIACTION_H

#include <moai-core/MOAIBlocker.h>
#include <moai-core/MOAIEventSource.h>

class MOAIAction;
class MOAIActionTree;

//================================================================//
// MOAIAction
//================================================================//
/**	@lua MOAIAction
	@text Base class for actions.
	
	@const	EVENT_START
	@const	EVENT_STOP		ID of event stop callback. Signature is: nil onStop ()
*/
class MOAIAction :
	public MOAIBlocker,
	public virtual MOAIInstanceEventSource {
private:
	
	u32 mPass; // total number of times the node has been updated
	
	MOAIAction*	mParent;
	
	typedef ZLLeanList < MOAIAction* >::Iterator ChildIt;
	ZLLeanList < MOAIAction* > mChildren;
	
	ZLLeanLink < MOAIAction* > mLink;
	
	ChildIt mNextChildIt; // this iterator is used when updating the action tree
	
	float	mThrottle;
	
	enum {
		FLAGS_AUTO_STOP			= 1 << 0,
		FLAGS_IS_PAUSED			= 1 << 1,
		FLAGS_IS_UPDATING		= 1 << 2,
	};
	
	u32 mActionFlags;
	
	//----------------------------------------------------------------//
	static int				_addChild				( lua_State* L );
	static int				_attach					( lua_State* L );
	static int				_clear					( lua_State* L );
	static int				_defer					( lua_State* L );
	static int				_detach					( lua_State* L );
	static int 				_getChildren			( lua_State* L );
	static int 				_hasChildren			( lua_State* L );
	static int				_isActive				( lua_State* L );
	static int				_isBusy					( lua_State* L );
	static int				_isDone					( lua_State* L );
	static int				_isPaused				( lua_State* L );
	static int				_pause					( lua_State* L );
	static int				_setAutoStop			( lua_State* L );
	static int				_start					( lua_State* L );
	static int				_stop					( lua_State* L );
	static int				_throttle				( lua_State* L );
	static int				_update					( lua_State* L );

	//----------------------------------------------------------------//
	void					ResetPass				( u32 pass = 0 );
	void					Update					( MOAIActionTree& tree, double step );

protected:

	//----------------------------------------------------------------//
	void					_RegisterLuaClass				( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_RegisterLuaFuncs				( RTTIVisitorHistory& history, MOAILuaState& state );
	virtual void			MOAIAction_DidLoseChild			( MOAIAction* child );
	virtual STLString		MOAIAction_GetDebugInfo			() const;
	virtual MOAIAction*		MOAIAction_GetDefaultParent		();
	virtual bool			MOAIAction_IsDone				();
	virtual void			MOAIAction_Start				();
	virtual void			MOAIAction_Stop					();
	virtual void			MOAIAction_Update				( double step );
	void					MOAIBlocker_Unblock				();
	
public:
	
	friend class MOAIActionTree;
	friend class MOAIActionMgr;
	
	DECL_LUA_FACTORY ( MOAIAction )
	
	enum {
		EVENT_ACTION_PRE_UPDATE,
		EVENT_ACTION_POST_UPDATE,
		EVENT_START,
		EVENT_STOP,
		TOTAL_EVENTS,
	};
	
	//----------------------------------------------------------------//
	void					Attach					( MOAIAction* parent, bool defer );
	void					ClearChildren			();
	void					Defer					( bool defer );
	void					Detach					();
	MOAIAction*				GetDefaultParent		();
	bool					IsActive				();
	bool					IsBusy					();
	bool					IsDone					();
	bool					IsPaused				();
							MOAIAction				();
							~MOAIAction				();
	void					Start					( MOAIAction* parent, bool defer );
	void					Stop					();
};

#endif
