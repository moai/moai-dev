// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIACTION_H
#define	MOAIACTION_H

#include <moaicore/MOAIBlocker.h>
#include <moaicore/MOAIEventSource.h>

//================================================================//
// MOAIAction
//================================================================//
/**	@name MOAIAction
	@text Base class for actions.
	
	@const	EVENT_STOP		ID of event stop callback. Signature is: nil onStop ()
*/
class MOAIAction :
	public MOAIBlocker,
	public MOAIEventSource {
private:
	
	bool	mNew;
	u32		mPass;
	
	MOAIAction* mParent;
	MOAIAction* mStackNext;
	USLeanLink < MOAIAction* > mLink;
	
	typedef USLeanList < MOAIAction* >::Iterator ChildIt;
	USLeanList < MOAIAction* > mChildren;
	
	float mThrottle;
	
	//----------------------------------------------------------------//
	static int			_addChild				( lua_State* L );
	static int			_clear					( lua_State* L );
	static int			_isActive				( lua_State* L );
	static int			_isBusy					( lua_State* L );
	static int			_isDone					( lua_State* L );
	static int			_start					( lua_State* L );
	static int			_stop					( lua_State* L );
	static int			_throttle				( lua_State* L );

	//----------------------------------------------------------------//
	void				OnUnblock				();
	void				Update					( float step, u32 pass, bool checkPass );

protected:

	//----------------------------------------------------------------//
	virtual void		OnStart					();
	virtual void		OnStop					();
	virtual void		OnUpdate				( float step );
	
public:
	
	friend class MOAIActionMgr;
	
	DECL_LUA_FACTORY ( MOAIAction )
	
	enum {
		EVENT_STOP,
		TOTAL_EVENTS,
	};
	
	//----------------------------------------------------------------//
	void				AddChild				( MOAIAction& action );
	void				ClearChildren			();
	bool				IsActive				();
	bool				IsBusy					();
	virtual bool		IsDone					();
						MOAIAction				();
						~MOAIAction				();
	void				RegisterLuaClass		( USLuaState& state );
	void				RegisterLuaFuncs		( USLuaState& state );
	void				RemoveChild				( MOAIAction& action );
	void				Start					();
	void				Start					( MOAIAction& parent );
	void				Stop					();
	STLString			ToString				();
};

#endif
