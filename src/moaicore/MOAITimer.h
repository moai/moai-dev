// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITIMER_H
#define	MOAITIMER_H

#include <moaicore/MOAIAction.h>
#include <moaicore/MOAINode.h>

class MOAIAnimCurve;

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
	
	@const	EVENT_KEYFRAME		ID of event stop callback. Signature is: nil onKeyframe ( number time, number value )
	@const	EVENT_TIMER_LOOP	ID of event loop callback. Signature is: nil onLoop ()
*/
class MOAITimer :
	public virtual MOAINode,
	public MOAIAction {
protected:

	USRef < MOAIAnimCurve >	mCurve;

	float	mStartTime;
	float	mEndTime;

	float	mTime;
	float	mSpeed;
	float	mDirection;

	u32		mMode;
	int		mTimesExecuted;

	//----------------------------------------------------------------//
	static int		_getTimesExecuted	( lua_State* L );
	static int		_setCurve			( lua_State* L );
	static int		_setMode			( lua_State* L );
	static int		_setSpan			( lua_State* L );
	static int		_setSpeed			( lua_State* L );
	static int		_setTime			( lua_State* L );
	
	//----------------------------------------------------------------//
	float			DoStep				( float step );
	void			GenerateCallbacks	( float t0, float t1, bool end );
	void			OnKeyframe			( u32 idx, float time, float value );
	void			OnLoop				();

public:
	
	DECL_LUA_FACTORY ( MOAITimer )
	
	enum {
		ATTR_TIME,
		TOTAL_ATTR,
	};
	
	enum {
		EVENT_TIMER_KEYFRAME = MOAIAction::TOTAL_EVENTS,
		EVENT_TIMER_LOOP,
		TOTAL_EVENTS,
	};
	
	enum TimerMode {
		NORMAL,
		REVERSE,
		LOOP,
		LOOP_REVERSE,
		PING_PONG,
	};
	
	//----------------------------------------------------------------//
	void			ApplyAttrOp			( u32 attrID, USAttrOp& attrOp );
	bool			IsDone				();
					MOAITimer			();
					~MOAITimer			();
	void			OnDepNodeUpdate		();
	void			OnStart				();
	void			OnUpdate			( float step );
	void			RegisterLuaClass	( USLuaState& state );
	void			RegisterLuaFuncs	( USLuaState& state );
	void			SetTime				( float time );
	STLString		ToString			();
};

#endif
