// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLBONE_H
#define	ZLBONE_H

#include <zl-util/ZLAccessors.h>
#include <zl-util/ZLAffine3D.h>
#include <zl-util/ZLMatrix3x3.h>
#include <zl-util/ZLMatrix4x4.h>
#include <zl-util/ZLQuaternion.h>

//================================================================//
// ZLBone
//================================================================//
class ZLBone {
private:

	float		mShearYX;
	float		mShearZX;

	float		mShearXY;
	float		mShearZY;
	
	float		mShearXZ;
	float		mShearYZ;

	ZLVec3D		mPivot;
	ZLVec3D		mLocation;
	ZLVec3D		mScale;

	union {
		struct {
			float	mX;
			float	mY;
			float	mZ;
		} mEuler;
		
		struct {
			float	mX;
			float	mY;
			float	mZ;
			float	mS;
		} mQuat;
	};

	u32		mRotationMode;

	//----------------------------------------------------------------//
	static float	ClampEuler			( float r );

public:

	enum {
		EULER_XYZ		= ( 2 << 0x04 ) + ( 1 << 0x02 ) + ( 0 << 0x00 ),
		EULER_XZY		= ( 1 << 0x04 ) + ( 2 << 0x02 ) + ( 0 << 0x00 ),
		EULER_YXZ		= ( 2 << 0x04 ) + ( 0 << 0x02 ) + ( 1 << 0x00 ),
		EULER_YZX		= ( 0 << 0x04 ) + ( 2 << 0x02 ) + ( 1 << 0x00 ),
		EULER_ZXY		= ( 1 << 0x04 ) + ( 0 << 0x02 ) + ( 2 << 0x00 ),
		EULER_ZYX		= ( 0 << 0x04 ) + ( 1 << 0x02 ) + ( 2 << 0x00 ),
		QUATERNION		= 0xffffffff,
	};

	IS ( Quaternion, mRotationMode, QUATERNION )

	//----------------------------------------------------------------//
	void			Compose				( ZLAffine3D& mtx );
	void			Compose				( ZLAffine3D& mtx, ZLAffine3D& inv );
	bool			GetEuler			( ZLVec3D& euler );
	bool			GetQuaternion		( ZLQuaternion& quat );
	void			SetRotationMode		( u32 mode );
					ZLBone				();
					~ZLBone				();
};

#endif
