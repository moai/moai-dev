// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIMCURVEQUAT_H
#define	MOAIANIMCURVEQUAT_H

#include <moai-sim/MOAIAnimCurveBase.h>
#include <moai-sim/MOAINode.h>

//================================================================//
// MOAIAnimCurveQuat
//================================================================//
/**	@name	MOAIAnimCurveQuat
	@text	Implementation of anim curve for rotation (via quaternion) values.
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

public:
	
	DECL_LUA_FACTORY ( MOAIAnimCurveQuat )
	
	//----------------------------------------------------------------//
	void			ApplyValueAttrOp		( MOAIAttrOp& attrOp, u32 op );
	void			GetDelta				( MOAIAttrOp& attrOp, const MOAIAnimKeySpan& span0, const MOAIAnimKeySpan& span1 ) const;
	ZLQuaternion	GetValue				( float time ) const;
	void			GetValue				( MOAIAttrOp& attrOp, const MOAIAnimKeySpan& span ) const;
	void			GetZero					( MOAIAttrOp& attrOp ) const;
					MOAIAnimCurveQuat		();
					~MOAIAnimCurveQuat		();
	void			OnDepNodeUpdate			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			ReserveSamples			( u32 total );
	void			SetSample				( u32 id, float x, float y, float z );
};

#endif
