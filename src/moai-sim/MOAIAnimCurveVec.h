// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIMCURVEVEC_H
#define	MOAIANIMCURVEVEC_H

#include <moai-sim/MOAIAnimCurve.h>

//================================================================//
// MOAIAnimCurveVec
//================================================================//
/**	@lua	MOAIAnimCurveVec
	@text	Implementation of animation curve for 3D vector values.
*/
class MOAIAnimCurveVec :
	public virtual MOAIAnimCurve {
private:

	ZLLeanArray < ZLVec3D > mSamples;
	ZLVec3D mValue;

	//----------------------------------------------------------------//
	static int		_getValueAtTime		( lua_State* L );
	static int		_setKey				( lua_State* L );

	//----------------------------------------------------------------//
	ZLVec3D			GetCurveDelta		() const;
	ZLVec3D			GetValue			( const MOAIAnimKeySpan& span ) const;

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
	
	DECL_LUA_FACTORY ( MOAIAnimCurveVec )
	
	//----------------------------------------------------------------//
	ZLVec3D			GetValue				( float time ) const;
					MOAIAnimCurveVec		();
					~MOAIAnimCurveVec		();
	void			ReserveSamples			( u32 total );
	void			SetSample				( ZLIndex idx, const ZLVec3D& value );
};

#endif
