// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIMCURVEFLOAT_H
#define	MOAIANIMCURVEFLOAT_H

#include <moai-core/MOAIAnimCurve.h>

//================================================================//
// MOAIAnimCurveFloat
//================================================================//
/**	@lua	MOAIAnimCurveFloat
	@text	Implementation of animation curve for floating point values.
*/
class MOAIAnimCurveFloat :
	public virtual MOAIAnimCurve {
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
	void			_RegisterLuaClass					( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs					( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAIAnimCurve_ApplyValueAttrOp		( ZLAttribute& attr, u32 op );
	void			MOAIAnimCurve_GetDelta				( ZLAttribute& attr, const MOAIAnimKeySpan& span0, const MOAIAnimKeySpan& span1 ) const;
	void			MOAIAnimCurve_GetValue				( ZLAttribute& attr, const MOAIAnimKeySpan& span ) const;
	void			MOAIAnimCurve_GetZero				( ZLAttribute& attr ) const;
	void			MOAIAnimCurve_ReserveSamples		( u32 total );
	void			MOAINode_Update						();

public:
	
	DECL_LUA_FACTORY ( MOAIAnimCurveFloat )
	
	//----------------------------------------------------------------//
	float			GetSample				( ZLIndex id );
	float			GetValue				( float time ) const;
	void			GetValueRange			( float t0, float t1, float &min, float &max );
					MOAIAnimCurveFloat		();
					~MOAIAnimCurveFloat		();
	void			SetSample				( ZLIndex idx, float value );
};

#endif
