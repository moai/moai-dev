// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIMCURVEVEC_H
#define	MOAIANIMCURVEVEC_H

#include <moai-sim/MOAIAnimCurveBase.h>
#include <moai-sim/MOAINode.h>

//================================================================//
// MOAIAnimCurveVec
//================================================================//
/**	@lua	MOAIAnimCurveVec
	@text	Implementation of animation curve for 3D vector values.
*/
class MOAIAnimCurveVec :
	public virtual MOAIAnimCurveBase {
private:

	ZLLeanArray < ZLVec3D > mSamples;
	ZLVec3D mValue;

	//----------------------------------------------------------------//
	static int		_getValueAtTime		( lua_State* L );
	static int		_setKey				( lua_State* L );

	//----------------------------------------------------------------//
	ZLVec3D			GetCurveDelta		() const;
	ZLVec3D			GetValue			( const MOAIAnimKeySpan& span ) const;

public:
	
	DECL_LUA_FACTORY ( MOAIAnimCurveVec )
	
	//----------------------------------------------------------------//
	void			ApplyValueAttrOp		( MOAIAttrOp& attrOp, u32 op );
	void			GetDelta				( MOAIAttrOp& attrOp, const MOAIAnimKeySpan& span0, const MOAIAnimKeySpan& span1 ) const;
	ZLVec3D			GetValue				( float time ) const;
	void			GetValue				( MOAIAttrOp& attrOp, const MOAIAnimKeySpan& span ) const;
	void			GetZero					( MOAIAttrOp& attrOp ) const;
					MOAIAnimCurveVec		();
					~MOAIAnimCurveVec		();
	void			OnDepNodeUpdate			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			ReserveSamples			( u32 total );
	void			SetSample				( u32 id, const ZLVec3D& value );
};

#endif
