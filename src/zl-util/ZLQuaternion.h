// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLQUATERNION_H
#define	ZLQUATERNION_H

#include <zl-util/ZLAffine3D.h>
#include <zl-util/ZLMatrix3x3.h>
#include <zl-util/ZLMatrix4x4.h>

//================================================================//
// ZLQuaternionStruct
//================================================================//
class ZLQuaternionStruct {
public:

	float				mS;
	ZLVec3DStruct		mV;

	//----------------------------------------------------------------//
	ZLQuaternionStruct operator + ( const ZLQuaternionStruct& v ) const {
		ZLQuaternionStruct result = *this;
		result.Add ( v );
		return result;
	}

	//----------------------------------------------------------------//
	ZLQuaternionStruct operator - ( const ZLQuaternionStruct& v ) const {
		ZLQuaternionStruct result = *this;
		result.Sub ( v );
		return result;
	}

	//----------------------------------------------------------------//
	void			Add					( const ZLQuaternionStruct& rhs );
	float			Dot					( const ZLQuaternionStruct& rhs ) const;
	void			Get					( ZLAffine3D& m ) const;
	void			Get					( ZLMatrix3x3& m ) const;
	void			Get					( ZLMatrix4x4& m ) const;
	void			Get					( ZLVec3D& axis, float& angle ) const;
	void			Get					( float& x, float& y, float& z ) const;
	void			Inverse				();
	float			Length				() const;
	float			LengthSquared		() const;
	void			Multiply			( const ZLQuaternionStruct& rhs );
	void			Normalize			();
	void			Scale				( float rhs );
	void			Slerp				( ZLQuaternionStruct q0, ZLQuaternionStruct q1, float t );
	void			Sub					( const ZLQuaternionStruct& rhs );
	ZLVec3D			Transform			( ZLVec3D loc ) const;
};

//================================================================//
// ZLQuaternion
//================================================================//
class ZLQuaternion :
	public ZLQuaternionStruct {
public:

	static const ZLQuaternion IDENT;

//	//----------------------------------------------------------------//
//	operator ZLQuaternionStruct () {
//	
//		return *( ZLQuaternionStruct* )this;
//	}

	//----------------------------------------------------------------//
					ZLQuaternion		();
					ZLQuaternion		( const ZLQuaternionStruct& quat );
					ZLQuaternion		( const ZLAffine3D& m );
					ZLQuaternion		( ZLMatrix3x3 m );
					ZLQuaternion		( const ZLMatrix4x4& m );
					ZLQuaternion		( const ZLVec3D& axis, float angle );
					ZLQuaternion		( float x, float y, float z ); // set from Euler angles
					ZLQuaternion		( float s, float x, float y, float z ); // set from quat
};

#endif
