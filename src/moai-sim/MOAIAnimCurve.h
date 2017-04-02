// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIMCURVE_H
#define	MOAIANIMCURVE_H

#include <moai-sim/MOAIAnimCurveBase.h>
#include <moai-sim/MOAINode.h>

//================================================================//
// MOAIAnimCurve
//================================================================//
/**	@lua	MOAIAnimCurve
	@text	Implementation of animation curve for floating point values.
*/
class MOAIAnimCurve :
	public virtual MOAIAnimCurveBase {
private:

	ZLLeanArray < float > mSamples;
	float mValue;

	//----------------------------------------------------------------//
	static int		_getValueAtTime		( lua_State* L );
	static int		_getValueRange		( lua_State* L );
	static int		_setKey				( lua_State* L );

	//----------------------------------------------------------------//
	float			GetCurveDelta		() const;
	float			GetValue			( const MOAIAnimKeySpan& span ) const;
	
	//----------------------------------------------------------------//
	void			MOAIAnimCurveBase_Accumulate		( MOAIAnimChannel& channel, float time, float opacity );
	void			MOAINode_Update						();

public:
	
	DECL_LUA_FACTORY ( MOAIAnimCurve )
	
	//----------------------------------------------------------------//
	void			ApplyValueAttrOp	( MOAIAttribute& attr, u32 op );
	void			Draw				( u32 resolution ) const;
	void			GetDelta			( MOAIAttribute& attr, const MOAIAnimKeySpan& span0, const MOAIAnimKeySpan& span1 ) const;
	float			GetSample			( u32 id );
	float			GetValue			( float time ) const;
	void			GetValue			( MOAIAttribute& attr, const MOAIAnimKeySpan& span ) const;
	void			GetValueRange		( float t0, float t1, float &min, float &max );
	void			GetZero				( MOAIAttribute& attr ) const;
					MOAIAnimCurve		();
					~MOAIAnimCurve		();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			ReserveSamples		( u32 total );
	void			SetSample			( u32 id, float value );
};

#endif
