// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIMCURVEBONE_H
#define	MOAIANIMCURVEBONE_H

#include <moai-sim/MOAIAnimCurveBase.h>
#include <moai-sim/MOAINode.h>

//================================================================//
// MOAIAnimCurveBone
//================================================================//
// TODO: doxygen
class MOAIAnimCurveBone :
	public virtual MOAIAnimCurveBase {
private:

	ZLLeanArray < ZLBone >				mSamples;

	//ZLLeanArray < ZLVec3D >			mPositionSamples;
	//ZLLeanArray < ZLQuaternion >	mRotationSamples;
	//ZLLeanArray < ZLVec3D >			mScaleSamples;
	
	ZLAffine3D mValue;

	//----------------------------------------------------------------//
	static int		_getValueAtTime		( lua_State* L );
	static int		_setKey				( lua_State* L );

	//----------------------------------------------------------------//
	//static ZLAffine3D		Compose				( const ZLVec3D& pos, const ZLQuaternion& rot, const ZLVec3D& scl );
	void					GetCurveDelta		( ZLBone& bone ) const;
	ZLAffine3D				GetValue			( const MOAIAnimKeySpan& span ) const;
	void					GetValue			( const MOAIAnimKeySpan& span, ZLBone& bone ) const;
	void					ReserveSamples		( u32 total );

	//----------------------------------------------------------------//
	void				MOAIAnimCurveBase_Accumulate		( MOAIAnimChannel& channel, float time, float opacity );
	void				MOAINode_Update						();

public:
	
	DECL_LUA_FACTORY ( MOAIAnimCurveBone )
	
	//----------------------------------------------------------------//
	void			ApplyValueAttrOp		( MOAIAttribute& attr, u32 op );
	void			GetDelta				( MOAIAttribute& attr, const MOAIAnimKeySpan& span0, const MOAIAnimKeySpan& span1 ) const;
	ZLAffine3D		GetValue				( float time ) const;
	void			GetValue				( MOAIAttribute& attr, const MOAIAnimKeySpan& span ) const;
	void			GetZero					( MOAIAttribute& attr ) const;
					MOAIAnimCurveBone		();
					~MOAIAnimCurveBone		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SetSample				( u32 idx, const ZLBone& bone );
};

#endif
