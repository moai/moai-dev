// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLQUATERNION_H
#define	ZLQUATERNION_H

#include <zl-core/ZLAffine3D.h>
#include <zl-core/ZLMatrix3x3.h>
#include <zl-core/ZLMatrix4x4.h>

//================================================================//
// ZLQuaternion
//================================================================//
class ZLQuaternion {
public:
	friend class MOAITest_USQuaternion;

	static const ZLQuaternion IDENT;

	ZLReal		mS;
	ZLVec3D		mV;

	//----------------------------------------------------------------//
	ZLQuaternion operator + ( const ZLQuaternion& v ) const {
		ZLQuaternion result = *this;
		result.Add ( v );
		return result;
	}

	//----------------------------------------------------------------//
	ZLQuaternion operator - ( const ZLQuaternion& v ) const {
		ZLQuaternion result = *this;
		result.Sub ( v );
		return result;
	}

	//----------------------------------------------------------------//
	void			Add					( const ZLQuaternion& rhs );
	ZLReal			Dot					( const ZLQuaternion& rhs ) const;
	void			Get					( ZLAffine3D& m ) const;
	void			Get					( ZLMatrix3x3& m ) const;
	void			Get					( ZLMatrix4x4& m ) const;
	void			Get					( ZLVec3D& axis, ZLReal& angle ) const;
	void			Get					( ZLReal& x, ZLReal& y, ZLReal& z ) const;
	void			Inverse				();
	ZLReal			Length				() const;
	ZLReal			LengthSquared		() const;
	void			Multiply			( const ZLQuaternion& rhs );
	void			Normalize			();
	void			Scale				( ZLReal rhs );
	void			Slerp				( ZLQuaternion q0, ZLQuaternion q1, ZLReal t );
	void			Sub					( const ZLQuaternion& rhs );
	ZLVec3D			Transform			( ZLVec3D loc ) const;
					ZLQuaternion		();
					ZLQuaternion		( const ZLAffine3D& m );
					ZLQuaternion		( ZLMatrix3x3 m );
					ZLQuaternion		( const ZLMatrix4x4& m );
					ZLQuaternion		( const ZLVec3D& axis, ZLReal angle );
					ZLQuaternion		( ZLReal x, ZLReal y, ZLReal z ); // set from Euler angles
					ZLQuaternion		( ZLReal s, ZLReal x, ZLReal y, ZLReal z ); // set from quat
};

#endif

