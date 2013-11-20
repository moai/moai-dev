// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIMCURVE_H
#define	MOAIANIMCURVE_H

#include <moaicore/MOAIAnimCurveBase.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAINode.h>
#include <moaicore/MOAIWeakPtr.h>

//================================================================//
// MOAIAnimCurve
//================================================================//
/**	@name	MOAIAnimCurve
	@text	Implementation of anim curve for floating point values.
*/
class MOAIAnimCurve :
	public virtual MOAIAnimCurveBase {
private:
	
	float mValue;

	//----------------------------------------------------------------//
	static int		_getValueAtTime		( lua_State* L );
	static int		_setKey				( lua_State* L );
	
protected:
	
	USLeanArray < float > mSamples;
	
	//----------------------------------------------------------------//
	virtual float	GetValue			( const MOAIAnimKeySpan& span );
	float			GetCurveDelta		() const;

public:
	
	DECL_LUA_FACTORY ( MOAIAnimCurve )
	
	//----------------------------------------------------------------//
	void			ApplyValueAttrOp	( MOAIAttrOp& attrOp, u32 op );
	void			Draw				( u32 resolution );
	void			GetDelta			( MOAIAttrOp& attrOp, const MOAIAnimKeySpan& span0, const MOAIAnimKeySpan& span1 );
	float			GetSample			( u32 id );
	float			GetValue			( float time );
	void			GetValue			( MOAIAttrOp& attrOp, const MOAIAnimKeySpan& span );
	void			GetZero				( MOAIAttrOp& attrOp ) const;
					MOAIAnimCurve		();
					~MOAIAnimCurve		();
	void			OnDepNodeUpdate		();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			ReserveSamples		( u32 total );
	void			SetSample			( u32 id, float value );
};

#endif