// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIACTION_H
#define	MOAIACTION_H

#include <moai-sim/MOAIBlocker.h>
#include <moai-sim/MOAINode.h>

class MOAIAction;
class MOAIActionTree;

//================================================================//
// MOAIActionStackMgr
//================================================================//
class MOAIActionStackMgr :
	public MOAIGlobalClass < MOAIActionStackMgr > {
private:
	
	ZLLeanStack < MOAIAction* > mStack;
	
	//----------------------------------------------------------------//
	MOAIAction*			GetCurrent				();
	MOAIAction*			GetDefaultParent		();
	void				Pop						();
	void				Push					( MOAIAction& action );
	
public:
	
	friend class MOAIAction;
	friend class MOAIActionTree;
	friend class MOAICoroutine;
	
	//----------------------------------------------------------------//
						MOAIActionStackMgr		();
						~MOAIActionStackMgr		();
};

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
	
	ChildIt mChildIt; // this iterator is used when updating the action tree
	
	float	mThrottle;
	
	enum {
		FLAGS_AUTO_STOP			= 1 << 0,
		FLAGS_IS_PAUSED			= 1 << 1,
		FLAGS_IS_UPDATING		= 1 << 2,
	};
	
	u32 mActionFlags;
	
	//----------------------------------------------------------------//
	static int			_addChild				( lua_State* L );
	static int			_attach					( lua_State* L );
	static int			_clear					( lua_State* L );
	static int			_defer					( lua_State* L );
	static int			_detach					( lua_State* L );
	static int			_isActive				( lua_State* L );
	static int			_isBusy					( lua_State* L );
	static int			_isDone					( lua_State* L );
	static int			_isPaused				( lua_State* L );
	static int			_pause					( lua_State* L );
	static int			_setAutoStop			( lua_State* L );
	static int			_start					( lua_State* L );
	static int			_stop					( lua_State* L );
	static int			_throttle				( lua_State* L );

	//----------------------------------------------------------------//
	virtual void		OnLostChild				( MOAIAction* child );
	void				OnUnblock				();
	void				ResetPass				( u32 pass = 0 );
	void				Update					( MOAIActionTree& tree, double step );

protected:

	//----------------------------------------------------------------//
	virtual STLString		GetDebugInfo			() const;
	virtual void			OnStart					();
	virtual void			OnStop					();
	virtual void			OnUpdate				( double step );
	
public:
	
	friend class MOAIActionTree;
	friend class MOAIActionStackMgr;
	
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
	virtual MOAIAction*		GetDefaultParent		();
	bool					IsActive				();
	bool					IsBusy					();
	virtual bool			IsDone					();
	bool					IsPaused				();
							MOAIAction				();
							~MOAIAction				();
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
	void					Start					( MOAIActionTree& tree, bool defer );
	void					Stop					();
};

#endif