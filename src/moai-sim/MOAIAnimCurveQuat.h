// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIMCURVEQUAT_H
#define	MOAIANIMCURVEQUAT_H

#include <moai-sim/MOAIAnimCurve.h>

//================================================================//
// MOAIAnimCurveQuat
//================================================================//
/**	@lua	MOAIAnimCurveQuat
	@text	Implementation of animation curve for rotation (via quaternion) values.
*/
class MOAIAnimCurveQuat :
	public virtual MOAIAnimCurve {
private:

	ZLLeanArray < ZLQuaternion > mSamples;
	ZLQuaternion mValue;

	//----------------------------------------------------------------//
	static int		_getValueAtTime		( lua_State* L );
	static int		_setKey				( lua_State* L );

	//----------------------------------------------------------------//
	ZLQuaternion	GetCurveDelta		() const;
	ZLQuaternion	GetValue			( const MOAIAnimKeySpan& span ) const;

	//----------------------------------------------------------------//
	void			MOAIAnimCurve_ApplyValueAttrOp		( ZLAttribute& attr, u32 op );
	void			MOAIAnimCurve_GetDelta				( ZLAttribute& attr, const MOAIAnimKeySpan& span0, const MOAIAnimKeySpan& span1 ) const;
	void			MOAIAnimCurve_GetValue				( ZLAttribute& attr, const MOAIAnimKeySpan& span ) const;
	void			MOAIAnimCurve_GetZero				( ZLAttribute& attr ) const;
	void			MOAIAnimCurve_ReserveSamples		( u32 total );
	void			MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
	void			MOAINode_Update						();

public:
	
	DECL_LUA_FACTORY ( MOAIAnimCurveQuat )
	
	//----------------------------------------------------------------//
	ZLQuaternion	GetValue				( float time ) const;
					MOAIAnimCurveQuat		();
					~MOAIAnimCurveQuat		();
	void			ReserveSamples			( u32 total );
	void			SetSample				( ZLIndex idx, float x, float y, float z );
};

#endif
