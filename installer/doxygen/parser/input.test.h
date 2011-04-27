// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITIMER_H
#define	MOAITIMER_H

#include <moaicore/MOAIAction.h>
#include <moaicore/MOAINode.h>

//================================================================//
// MOAITimer
//================================================================//
/**	@name	MOAITimer
	@text	Timer class for driving curves and animations.

	@attr	ATTR_TIME
	
	@const	NORMAL
	@const	REVERSE
	@const	LOOP
	@const	LOOP_REVERSE
	@const	PING_PONG
*/
class MOAITimer :
	public virtual MOAINode,
	public MOAIAction {
protected:

	float	mStartTime;
	float	mEndTime;

	float	mTime;
	float	mSpeed;
	float	mDirection;

	u32		mMode;
	int		mTimesExecuted;

	USLuaRef	mCallback;

	//----------------------------------------------------------------//
	static int	_getTimesExecuted	( lua_State* L );
	static int	_setCallback		( lua_State* L );
	static int	_setMode			( lua_State* L );
	static int	_setSpan			( lua_State* L );
	static int	_setSpeed			( lua_State* L );
	static int	_setTime			( lua_State* L );
	
	//----------------------------------------------------------------//
	void		Callback			();
	float		DoStep				( float step );

public:
	
	DECL_LUA_FACTORY ( MOAITimer )
	
	enum {
		ATTR_TIME,
		TOTAL_ATTR,
	};
	
	enum TimerMode {
		NORMAL,
		REVERSE,
		LOOP,
		LOOP_REVERSE,
		PING_PONG,
	};
	
	//----------------------------------------------------------------//
	void		ApplyAttrOp			( u32 attrID, USAttrOp& attrOp );
	bool		IsDone				();
				MOAITimer			();
				~MOAITimer			();
	void		OnDepNodeUpdate		();
	void		OnStart				();
	void		OnUpdate			( float step );
	void		RegisterLuaClass	( USLuaState& state );
	void		RegisterLuaFuncs	( USLuaState& state );
	void		SetTime				( float time );
	STLString	ToString			();
};

#endif
