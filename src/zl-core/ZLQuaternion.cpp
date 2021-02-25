// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-core/ZLQuaternion.h>

//================================================================//
// ZLQuaternion
//================================================================//

const ZLQuaternion ZLQuaternion::IDENT = ZLQuaternion ( 1.0, 0.0, 0.0, 0.0 );

//----------------------------------------------------------------//
void ZLQuaternion::Add ( const ZLQuaternion& rhs ) {
	
	mS += rhs.mS;
	mV.mX += rhs.mV.mX;
	mV.mY += rhs.mV.mY;
	mV.mZ += rhs.mV.mZ;
}

//----------------------------------------------------------------//
ZLReal ZLQuaternion::Dot ( const ZLQuaternion& rhs ) const {
	return ( mS * rhs.mS ) + mV.Dot ( rhs.mV );
}

//----------------------------------------------------------------//
void ZLQuaternion::Get ( ZLAffine3D& m ) const {
	
	ZLMatrix3x3 affine;
	this->Get ( affine );
	m = ZLAffine3D ( affine );
}

//----------------------------------------------------------------//
void ZLQuaternion::Get ( ZLMatrix3x3& m ) const {
	
	ZLReal x2 = mV.mX + mV.mX;
	ZLReal y2 = mV.mY + mV.mY;
	ZLReal z2 = mV.mZ + mV.mZ;
	ZLReal xx = mV.mX * x2;
	ZLReal xy = mV.mX * y2;
	ZLReal xz = mV.mX * z2;
	ZLReal yy = mV.mY * y2;
	ZLReal yz = mV.mY * z2;
	ZLReal zz = mV.mZ * z2;
	ZLReal wx = mS * x2;
	ZLReal wy = mS * y2;
	ZLReal wz = mS * z2;

	m.m [ ZLMatrix3x3::C0_R0 ] = 1.0 - ( yy + zz );
	m.m [ ZLMatrix3x3::C0_R1 ] = xy - wz;
	m.m [ ZLMatrix3x3::C0_R2 ] = xz + wy;

	m.m [ ZLMatrix3x3::C1_R0 ] = xy + wz;
	m.m [ ZLMatrix3x3::C1_R1 ] = 1.0 - ( xx + zz );
	m.m [ ZLMatrix3x3::C1_R2 ] = yz - wx;

	m.m [ ZLMatrix3x3::C2_R0 ] = xz - wy;
	m.m [ ZLMatrix3x3::C2_R1 ] = yz + wx;
	m.m [ ZLMatrix3x3::C2_R2 ] = 1.0 - ( xx + yy );
	
	m.Transpose ();
}

//----------------------------------------------------------------//
void ZLQuaternion::Get ( ZLMatrix4x4& m ) const {
	
	ZLMatrix3x3 mtx3x3;
	this->Get ( mtx3x3 );
	m = ZLMatrix4x4 ( mtx3x3 );
}

//----------------------------------------------------------------//
void ZLQuaternion::Get ( ZLVec3D& axis, ZLReal& angle ) const {

	ZLReal sqrLen = ( mV.mX * mV.mX ) + ( mV.mY * mV.mY ) + ( mV.mZ * mV.mZ );

	if ( sqrLen > 0.0 ) {
		angle = 2.0 * ( ZLReal )( acos ( mS ) * R2D );
		sqrLen = 1.0 / ( ZLReal )sqrt ( sqrLen );
		axis.mX = mV.mX * sqrLen;
		axis.mY = mV.mY * sqrLen;
		axis.mZ = mV.mZ * sqrLen;
	}
	else {
		angle = 0.0;
		axis.mX = 0.0;
		axis.mY = 1.0;
		axis.mZ = 0.0;
	}
}

//----------------------------------------------------------------//
void ZLQuaternion::Get ( ZLReal& x, ZLReal& y, ZLReal& z ) const {

	ZLReal sz_xy = 2.0 * ( mS * mV.mZ + mV.mX * mV.mY );

	if ( sz_xy > 1.0 ) {
		sz_xy = 1.0;
	}
	else if ( sz_xy < -1.0 ) {
		sz_xy = -1.0;
	}

	ZLReal sx_yz = 2.0 * ( mS * mV.mX - mV.mY * mV.mZ );
	ZLReal xx_zz = 1.0 - 2.0 * ( mV.mX * mV.mX + mV.mZ * mV.mZ );

	ZLReal sy_xz = 2.0 * ( mS * mV.mY - mV.mX * mV.mZ );
	ZLReal yy_zz = 1.0 - 2.0 * ( mV.mY * mV.mY + mV.mZ * mV.mZ );

	//round values to prevent floating errors affecting results
	if ( sx_yz < EPSILON && sx_yz > - EPSILON ) {
		sx_yz = 0.0;
	}
	if ( xx_zz < EPSILON && xx_zz > - EPSILON ) {
		xx_zz = 0.0;
	}

	if ( sy_xz < EPSILON && sy_xz > - EPSILON ) {
		sy_xz = 0.0;
	}
	if ( yy_zz < EPSILON && yy_zz > - EPSILON ) {
		yy_zz = 0.0;
	}

	x = ( ZLReal )( atan2 ( sy_xz, yy_zz ) * R2D );
	y = ( ZLReal )( asin ( sz_xy ) * R2D );
	z = ( ZLReal )( atan2 ( sx_yz, xx_zz ) * R2D );

}

//----------------------------------------------------------------//
void ZLQuaternion::Inverse () {
	
	ZLReal length = Length ();
	
	mS = mS / length;
	mV.mX = mV.mX / length;
	mV.mY = mV.mY / length;
	mV.mZ = mV.mZ / length;
}

//----------------------------------------------------------------//
ZLReal ZLQuaternion::Length () const {
	return sqrtf ( LengthSquared ());
}

//----------------------------------------------------------------//
ZLReal ZLQuaternion::LengthSquared () const {
	return  ( mS * mS ) + ( mV.mX * mV.mX ) + ( mV.mY * mV.mY ) + ( mV.mZ * mV.mZ );
}

//----------------------------------------------------------------//
void ZLQuaternion::Multiply ( const ZLQuaternion& rhs ) {

	ZLVec3D cross;
	cross.Cross ( mV, rhs.mV );
	
	ZLVec3D resultVec;
	resultVec.Init ( rhs.mV );
	resultVec.Scale ( mS );
	
	ZLVec3D scaledVec;
	scaledVec.Init ( mV );
	scaledVec.Scale ( rhs.mS );
	
	resultVec.Add ( scaledVec );
	resultVec.Sub ( cross );
	
	mS = mS * rhs.mS - mV.Dot ( rhs.mV );
	mV.Init ( resultVec );
	
}

//----------------------------------------------------------------//
void ZLQuaternion::Normalize() {
	
	ZLReal length = Length ();
	
	mS /= length;
	mV.mX /= length;
	mV.mY /= length;
	mV.mZ /= length;
}

//----------------------------------------------------------------//
void ZLQuaternion::Scale ( ZLReal rhs ) {
	
	mV.mX *= rhs;
	mV.mY *= rhs;
	mV.mZ *= rhs;
	mS *= rhs;
}

//----------------------------------------------------------------//
void ZLQuaternion::Slerp ( ZLQuaternion q0, ZLQuaternion q1, ZLReal t ) {
	
	ZLReal floatRound = q0.Dot ( q1 );
	
	if ( floatRound > 1.0 ) {
		floatRound = 1.0;
	}
	
	ZLReal angle = ( ZLReal )acos ( floatRound );
	
	if ( !( angle < 0.001 && angle > -0.001 )) {
		
		q0.Scale (( ZLReal )sin ( angle * ( 1 - t )));
		q1.Scale (( ZLReal )sin ( angle * t ));
	
		q0.Add ( q1 );
		
		q0.Scale ( 1.0 / ( ZLReal )sin ( angle ));
	}
	
	*this = q0;
}

//----------------------------------------------------------------//
void ZLQuaternion::Sub ( const ZLQuaternion& rhs ) {
	
	mS -= rhs.mS;
	mV.mX -= rhs.mV.mX;
	mV.mY -= rhs.mV.mY;
	mV.mZ -= rhs.mV.mZ;
}

//----------------------------------------------------------------//
ZLVec3D ZLQuaternion::Transform ( ZLVec3D loc ) const {

	ZLQuaternion r ( 0.0, loc.mX, loc.mY, loc.mZ );
	
	ZLQuaternion inv;
	inv = *this;
	inv.Inverse();
	
	inv.Multiply( r );
	inv.Multiply( *this );
	
	return inv.mV;
}

//----------------------------------------------------------------//
ZLQuaternion::ZLQuaternion () {
}

//----------------------------------------------------------------//
ZLQuaternion::ZLQuaternion ( const ZLAffine3D& m ) {

	*this = ZLQuaternion ( ZLMatrix3x3 ( m ));
}

//----------------------------------------------------------------//
ZLQuaternion::ZLQuaternion ( ZLMatrix3x3 m ) {

	m.Transpose ();

	ZLReal trace = m.m [ ZLMatrix3x3::C0_R0 ] + m.m [ ZLMatrix3x3::C1_R1 ] + m.m [ ZLMatrix3x3::C2_R2 ] + 1.0;

	if ( trace > 1.001 ) {
		
		ZLReal sqr = 2.0 * sqrtf ( trace );
		mS = 0.25 * sqr;
		mV.mX = ( m.m [ ZLMatrix3x3::C2_R1 ] - m.m [ ZLMatrix3x3::C1_R2 ] ) / sqr;
		mV.mY = ( m.m [ ZLMatrix3x3::C0_R1 ] - m.m [ ZLMatrix3x3::C2_R1 ] ) / sqr;
		mV.mZ = ( m.m [ ZLMatrix3x3::C1_R0 ] - m.m [ ZLMatrix3x3::C0_R1 ] ) / sqr;
	}
	else {
		
		if (( m.m [ ZLMatrix3x3::C0_R0 ] > m.m [ ZLMatrix3x3::C1_R1 ] ) && ( m.m [ ZLMatrix3x3::C0_R0 ] > m.m [ ZLMatrix3x3::C2_R2 ] )) {
	  
			ZLReal sqr = ( ZLReal ) sqrt( 1.0 + m.m [ ZLMatrix3x3::C0_R0 ] - m.m [ ZLMatrix3x3::C1_R1 ] - m.m [ ZLMatrix3x3::C2_R2 ] ) * 2.0; // S=4*qx
			mS = ( m.m [ ZLMatrix3x3::C2_R1 ] - m.m [ ZLMatrix3x3::C1_R2 ] ) / sqr;
			mV.mX = 0.25 * sqr;
			mV.mY = ( m.m [ ZLMatrix3x3::C0_R1 ] + m.m [ ZLMatrix3x3::C1_R0 ] ) / sqr;
	    	mV.mZ = ( m.m [ ZLMatrix3x3::C0_R2 ] + m.m [ ZLMatrix3x3::C2_R0 ] ) / sqr;
		}
		else if ( m.m [ ZLMatrix3x3::C1_R1 ] > m.m [ ZLMatrix3x3::C2_R2 ] ) {
	
			ZLReal sqr = ( ZLReal ) sqrt( 1.0 + m.m [ ZLMatrix3x3::C1_R1 ] - m.m [ ZLMatrix3x3::C0_R0 ] - m.m [ ZLMatrix3x3::C2_R2 ] ) * 2.0; // S=4*qy
			mS = ( m.m [ ZLMatrix3x3::C0_R2 ] - m.m [ ZLMatrix3x3::C2_R0 ] ) / sqr;
			mV.mX = ( m.m [ ZLMatrix3x3::C0_R1 ] + m.m [ ZLMatrix3x3::C1_R0 ] ) / sqr;
			mV.mY = 0.25 * sqr;
			mV.mZ = ( m.m [ ZLMatrix3x3::C1_R2 ] + m.m [ ZLMatrix3x3::C2_R1 ] ) / sqr;
		}
		else {
			
			ZLReal sqr = ( ZLReal ) sqrt( 1.0 + m.m [ ZLMatrix3x3::C2_R2 ] - m.m [ ZLMatrix3x3::C0_R0 ] - m.m [ ZLMatrix3x3::C1_R1 ] ) * 2.0; // S=4*qz
			mS = ( m.m [ ZLMatrix3x3::C1_R0 ] - m.m [ ZLMatrix3x3::C0_R1 ] ) / sqr;
			mV.mX = ( m.m [ ZLMatrix3x3::C0_R2 ] + m.m [ ZLMatrix3x3::C2_R0 ] ) / sqr;
			mV.mY = ( m.m [ ZLMatrix3x3::C1_R2 ] + m.m [ ZLMatrix3x3::C2_R1 ] ) / sqr;
			mV.mZ = 0.25 * sqr;
		}
	}
	
	this->Normalize ();
}

//----------------------------------------------------------------//
ZLQuaternion::ZLQuaternion ( const ZLMatrix4x4& m ) {

	*this = ZLQuaternion ( ZLMatrix3x3 ( m ));
}

//----------------------------------------------------------------//
ZLQuaternion::ZLQuaternion ( const ZLVec3D& axis, ZLReal angle ) {

	angle *= ( ZLReal )D2R;

	ZLReal s = ( ZLReal ) sin ( angle / 2.0 );

	mS = ( ZLReal ) cos ( angle / 2.0 );
	mV.mX = axis.mX * s;
	mV.mY = axis.mY * s;
	mV.mZ = axis.mZ * s;
}

//----------------------------------------------------------------//
ZLQuaternion::ZLQuaternion ( ZLReal x, ZLReal y, ZLReal z ) {
	
	x *= ( ZLReal )D2R;
	y *= ( ZLReal )D2R;
	z *= ( ZLReal )D2R;
	
	ZLReal cx = Cos ( x / 2.0 );
	ZLReal cy = Cos ( y / 2.0 );
	ZLReal cz = Cos ( z / 2.0 );
	
	ZLReal sx = Sin ( x / 2.0 );
	ZLReal sy = Sin ( y / 2.0 );
	ZLReal sz = Sin ( z / 2.0 );
	
	ZLReal cycz = cy * cz;
	ZLReal sysz = sy * sz;
	ZLReal sycz = sy * cz;
	ZLReal cysz = cy * sz;

	mS		= ( cx * cycz ) - ( sx * sysz );
	mV.mX	= ( sx * sycz ) + ( cx * cysz );
	mV.mY	= ( sx * cycz ) + ( cx * sysz );
	mV.mZ	= ( cx * sycz ) - ( sx * cysz );
}

//----------------------------------------------------------------//
ZLQuaternion::ZLQuaternion ( ZLReal s, ZLReal x, ZLReal y, ZLReal z ) {

	mS = s;
	mV.mX = x;
	mV.mY = y;
	mV.mZ = z;
}
