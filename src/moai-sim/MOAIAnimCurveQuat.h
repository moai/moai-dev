// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIMCURVEQUAT_H
#define	MOAIANIMCURVEQUAT_H

#include <moai-sim/MOAIAnimCurveBase.h>
#include <moai-sim/MOAINode.h>

//================================================================//
// MOAIAnimCurveQuat
//================================================================//
/**	@lua	MOAIAnimCurveQuat
	@text	Implementation of animation curve for rotation (via quaternion) values.
*/
class MOAIAnimCurveQuat :
	public virtual MOAIAnimCurveBase {
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
	void			MOAIAnimCurveBase_Accumulate		( MOAIAnimChannel& channel, float time, float opacity );
	void			MOAINode_Update						();

public:
	
	DECL_LUA_FACTORY ( MOAIAnimCurveQuat )
	
	//----------------------------------------------------------------//
	void			ApplyValueAttrOp		( MOAIAttribute& attr, u32 op );
	void			GetDelta				( MOAIAttribute& attr, const MOAIAnimKeySpan& span0, const MOAIAnimKeySpan& span1 ) const;
	ZLQuaternion	GetValue				( float time ) const;
	void			GetValue				( MOAIAttribute& attr, const MOAIAnimKeySpan& span ) const;
	void			GetZero					( MOAIAttribute& attr ) const;
					MOAIAnimCurveQuat		();
					~MOAIAnimCurveQuat		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			ReserveSamples			( u32 total );
	void			SetSample				( u32 id, float x, float y, float z );
};

#endif
