// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLQUATERNION_H
#define	ZLQUATERNION_H

#include <zl-util/ZLAffine3D.h>
#include <zl-util/ZLMatrix4x4.h>

//================================================================//
// ZLQuaternion
//================================================================//
class ZLQuaternion {
private:
	friend class MOAITest_USQuaternion;

	real		mS;
	ZLVec3D		mV;

public:

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
	real			Dot					( const ZLQuaternion& rhs ) const;
	void			Get					( ZLAffine3D& m ) const;
	void			Get					( ZLMatrix4x4& m ) const;
	void			Get					( ZLVec3D& axis, real& angle ) const;
	void			Get					( real& x, real& y, real& z ) const;
	void			Identity			();
	void			Inverse				();
	real			Length				() const;
	real			LengthSquared		() const;
	void			Multiply			( const ZLQuaternion& rhs );
	void			Normalize			();
	void			Scale				( real rhs );
	void			Set					( const ZLAffine3D& m );
	void			Set					( const ZLMatrix4x4& m );
	void			Set					( const ZLVec3D& axis, real angle );
	void			Set					( real x, real y, real z ); // set from Euler angles
	void			Set					( real s, real x, real y, real z ); // set from quat
	void			Slerp				( ZLQuaternion q0, ZLQuaternion q1, real t );
	void			Sub					( const ZLQuaternion& rhs );
	ZLVec3D			Transform			( ZLVec3D loc ) const;
};

#endif