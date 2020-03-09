// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIMCURVEBONE_H
#define	MOAIANIMCURVEBONE_H

#include <moai-sim/MOAIAnimCurve.h>

//================================================================//
// MOAIAnimCurveBone
//================================================================//
// TODO: doxygen
class MOAIAnimCurveBone :
	public virtual MOAIAnimCurve {
private:

	ZLLeanArray < ZLVec3D >			mPositionSamples;
	ZLLeanArray < ZLQuaternion >	mRotationSamples;
	ZLLeanArray < ZLVec3D >			mScaleSamples;
	
	ZLAffine3D mValue;

	//----------------------------------------------------------------//
	static int		_getValueAtTime		( lua_State* L );
	static int		_setKey				( lua_State* L );

	//----------------------------------------------------------------//
	static ZLAffine3D		Compose				( const ZLVec3D& pos, const ZLQuaternion& rot, const ZLVec3D& scl );
	void					GetCurveDelta		( ZLVec3D& pos, ZLQuaternion& rot, ZLVec3D& scl ) const;
	ZLAffine3D				GetValue			( const MOAIAnimKeySpan& span ) const;
	void					GetValue			( const MOAIAnimKeySpan& span, ZLVec3D& pos, ZLQuaternion& rot, ZLVec3D& scl ) const;
	void					ReserveSamples		( u32 total );

	//----------------------------------------------------------------//
	void			MOAIAnimCurve_ApplyValueAttrOp		( ZLAttribute& attr, u32 op );
	void			MOAIAnimCurve_GetDelta				( ZLAttribute& attr, const MOAIAnimKeySpan& span0, const MOAIAnimKeySpan& span1 ) const;
	void			MOAIAnimCurve_GetValue				( ZLAttribute& attr, const MOAIAnimKeySpan& span ) const;
	void			MOAIAnimCurve_GetZero				( ZLAttribute& attr ) const;
	void			MOAIAnimCurve_ReserveSamples		( u32 total );
	void			MOAILuaObject_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAINode_Update						();

public:
	
	DECL_LUA_FACTORY ( MOAIAnimCurveBone )
	
	//----------------------------------------------------------------//
	ZLAffine3D		GetValue				( float time ) const;
					MOAIAnimCurveBone		();
					~MOAIAnimCurveBone		();
	void			SetSamplePosition		( ZLIndex idx, float x, float y, float z );
	void			SetSampleRotation		( ZLIndex idx, float x, float y, float z, float w );
	void			SetSampleScale			( ZLIndex idx, float x, float y, float z );
};

#endif
