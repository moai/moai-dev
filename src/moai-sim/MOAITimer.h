// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITIMER_H
#define	MOAITIMER_H

#include <moai-sim/MOAIAction.h>

class MOAIAnimCurveFloat;

//================================================================//
// MOAITimer
//================================================================//
/**	@lua	MOAITimer
	@text	Timer class for driving curves and animations.

	@attr	ATTR_TIME
	
	@const	NORMAL
	@const	REVERSE
	@const	CONTINUE
	@const	CONTINUE_REVERSE
	@const	LOOP
	@const	LOOP_REVERSE
	@const	PING_PONG
	
	@const	EVENT_TIMER_KEYFRAME		ID of event stop callback. Signature is: nil onKeyframe ( MOAITimer self, number keyframe, number timesExecuted, number time, number value )
	@const	EVENT_TIMER_LOOP			ID of event loop callback. Signature is: nil onLoop ( MOAITimer self, number timesExecuted )
	@const	EVENT_TIMER_BEGIN_SPAN		Called when timer starts or after roll over (if looping). Signature is: nil onBeginSpan ( MOAITimer self, number timesExecuted )
	@const	EVENT_TIMER_END_SPAN		Called when timer ends or before roll over (if looping). Signature is: nil onEndSpan ( MOAITimer self, number timesExecuted )
*/
class MOAITimer :
	public virtual MOAINode,
	public MOAIAction {
protected:

	MOAILuaSharedPtr < MOAIAnimCurveFloat > mCurve;

	float	mTime;
	float	mCycle;
	
	//----------------------------------------------------------------//
	static int		_getSpeed			( lua_State* L );
	static int		_getTime			( lua_State* L );
	static int		_getTimesExecuted	( lua_State* L );
	static int		_reset				( lua_State* L );
	static int		_setCurve			( lua_State* L );
	static int		_setMode			( lua_State* L );
	static int		_setSpan			( lua_State* L );
	static int		_setSpeed			( lua_State* L );
	static int		_setTime			( lua_State* L );
	static int		_toggleDirection	( lua_State* L );
	
	//----------------------------------------------------------------//
	void			GenerateKeyframeCallbacks	( float t0, float t1, bool end );
	void			OnBeginSpan					();
	void			OnEndSpan					();
	void			OnKeyframe					( u32 idx, float time, float value );
	void			OnLoop						();

	//----------------------------------------------------------------//
	bool			MOAIAction_IsDone			();
	void			MOAIAction_Start			();
	void			MOAIAction_Update			( double step );
	bool			MOAINode_ApplyAttrOp		( ZLAttrID attrID, ZLAttribute& attr, u32 op );

protected:

	float	mSpeed;
	float	mDirection;

	u32		mMode;
	float	mTimesExecuted;

	float	mStartTime;
	float	mEndTime;

public:
	
	DECL_LUA_FACTORY ( MOAITimer )
	DECL_ATTR_HELPER ( MOAITimer )
	
	enum {
		ATTR_TIME,
		TOTAL_ATTR,
	};
	
	enum {
		EVENT_TIMER_KEYFRAME = MOAIAction::TOTAL_EVENTS,
		EVENT_TIMER_LOOP,
		EVENT_TIMER_BEGIN_SPAN,
		EVENT_TIMER_END_SPAN,
		TOTAL_EVENTS,
	};
	
	enum TimerMode {
		NORMAL,
		REVERSE,
		CONTINUE,
		CONTINUE_REVERSE,
		LOOP,
		LOOP_REVERSE,
		PING_PONG,
	};
	
	//----------------------------------------------------------------//
	void			DoStep				( float step );
	float			GetCycle			();
	float			GetLength			();
	float			GetNormalizedTime	();
	float			GetTime				();
					MOAITimer			();
					~MOAITimer			();
	void			MOAILuaObject_RegisterLuaClass	( MOAIComposer& composer, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs	( MOAIComposer& composer, MOAILuaState& state );
	void			SetSpan				( float span );
	void			SetSpan				( float startTime, float endTime );
	void			SetTime				( float time );
	void			ToggleDirection		();
};

#endif
