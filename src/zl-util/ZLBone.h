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
protected:

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
		ZLVec3DStruct			mEuler;
		ZLQuaternionStruct		mQuaternion;
	};

	u32		mRotationMode;

	//----------------------------------------------------------------//
	static float	ClampEuler			( float r );

public:

	static const ZLBone IDENT;

	enum {
		EULER_XYZ		= ( 2 << 0x04 ) + ( 1 << 0x02 ) + ( 0 << 0x00 ),
		EULER_XZY		= ( 1 << 0x04 ) + ( 2 << 0x02 ) + ( 0 << 0x00 ),
		EULER_YXZ		= ( 2 << 0x04 ) + ( 0 << 0x02 ) + ( 1 << 0x00 ),
		EULER_YZX		= ( 0 << 0x04 ) + ( 2 << 0x02 ) + ( 1 << 0x00 ),
		EULER_ZXY		= ( 1 << 0x04 ) + ( 0 << 0x02 ) + ( 2 << 0x00 ),
		EULER_ZYX		= ( 0 << 0x04 ) + ( 1 << 0x02 ) + ( 2 << 0x00 ),
		QUATERNION		= 0xffffffff,
	};

	GET_SET ( ZLVec3D, Pivot, mPivot )
	GET_SET ( ZLVec3D, Location, mLocation )
	GET_SET ( ZLVec3D, Scale, mScale )

	IS ( Quaternion, mRotationMode, QUATERNION )

	//----------------------------------------------------------------//
	void			Add					( const ZLBone& rhs );
	void			Compose				( ZLAffine3D& mtx ) const;
	void			Compose				( ZLAffine3D& mtx, ZLAffine3D& inv ) const;
	void			ComposeRotation		( ZLMatrix3x3& mtx ) const;
	ZLVec3D			GetEuler			() const;
	ZLQuaternion	GetQuaternion		() const;
	void			Ident				();
	void			Interpolate			( const ZLBone& b0, const ZLBone& b1, float t );
	void			SetEuler			( float x, float y, float z, u32 mode = EULER_XYZ );
	void			SetLocation			( float x, float y, float z );
	void			SetPivot			( float x, float y, float z );
	void			SetQuaternion		( const ZLQuaternion& quaternion );
	void			SetQuaternion		( float s, float x, float y, float z );
	void			SetRotationMode		( u32 mode );
	void			SetScale			( float x, float y, float z );
	void			Sub					( const ZLBone& rhs );
					ZLBone				();
					~ZLBone				();
};

#endif
