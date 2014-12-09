
#include "pch.h"
#include <zl-util/ZLQuaternion.h>

//================================================================//
// ZLQuaternion
//================================================================//

//----------------------------------------------------------------//
void ZLQuaternion::Add ( const ZLQuaternion& rhs ) {
	
	mS += rhs.mS;
	mV.mX += rhs.mV.mX;
	mV.mY += rhs.mV.mY;
	mV.mZ += rhs.mV.mZ;
}

//----------------------------------------------------------------//
real ZLQuaternion::Dot ( const ZLQuaternion& rhs ) const {
	return ( mS * rhs.mS ) + mV.Dot ( rhs.mV );
}

//----------------------------------------------------------------//
void ZLQuaternion::Get ( ZLAffine3D& m ) const {
	
	real x2 = mV.mX + mV.mX;
	real y2 = mV.mY + mV.mY;
	real z2 = mV.mZ + mV.mZ;
	real xx = mV.mX * x2;
	real xy = mV.mX * y2;
	real xz = mV.mX * z2;
	real yy = mV.mY * y2;
	real yz = mV.mY * z2;
	real zz = mV.mZ * z2;
	real wx = mS * x2;
	real wy = mS * y2;
	real wz = mS * z2;

	m.m [ ZLAffine3D::C0_R0 ] = 1.0 - ( yy + zz );
	m.m [ ZLAffine3D::C0_R1 ] = xy - wz;
	m.m [ ZLAffine3D::C0_R2 ] = xz + wy;

	m.m [ ZLAffine3D::C1_R0 ] = xy + wz;
	m.m [ ZLAffine3D::C1_R1 ] = 1.0 - ( xx + zz );
	m.m [ ZLAffine3D::C1_R2 ] = yz - wx;

	m.m [ ZLAffine3D::C2_R0 ] = xz - wy;
	m.m [ ZLAffine3D::C2_R1 ] = yz + wx;
	m.m [ ZLAffine3D::C2_R2 ] = 1.0 - ( xx + yy );

	m.m [ ZLAffine3D::C3_R0 ] = 0.0;
	m.m [ ZLAffine3D::C3_R1 ] = 0.0;
	m.m [ ZLAffine3D::C3_R2 ] = 0.0;
}

//----------------------------------------------------------------//
void ZLQuaternion::Get ( ZLMatrix4x4& m ) const {
	
	ZLAffine3D affine;
	this->Get ( affine );
	m.Init ( affine );
}

//----------------------------------------------------------------//
void ZLQuaternion::Get ( ZLVec3D& axis, real& angle ) const {

	real sqrLen = ( mV.mX * mV.mX ) + ( mV.mY * mV.mY ) + ( mV.mZ * mV.mZ );

	if ( sqrLen > 0.0 ) {
		angle = 2.0 * ( real )( acos ( mS ) * R2D );
		sqrLen = 1.0 / ( real )sqrt ( sqrLen );
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

void ZLQuaternion::Get ( real& x, real& y, real& z ) const {

	real sz_xy = 2.0 * ( mS * mV.mZ + mV.mX * mV.mY );

	if ( sz_xy > 1.0 ) {
		sz_xy = 1.0;
	}
	else if ( sz_xy < -1.0 ) {
		sz_xy = -1.0;
	}

	real sx_yz = 2.0 * ( mS * mV.mX - mV.mY * mV.mZ );
	real xx_zz = 1.0 - 2.0 * ( mV.mX * mV.mX + mV.mZ * mV.mZ );

	real sy_xz = 2.0 * ( mS * mV.mY - mV.mX * mV.mZ );
	real yy_zz = 1.0 - 2.0 * ( mV.mY * mV.mY + mV.mZ * mV.mZ );

	//round values to prevent realing errors affecting results
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

	x = ( real )( atan2 ( sy_xz, yy_zz ) * R2D );
	y = ( real )( asin ( sz_xy ) * R2D );
	z = ( real )( atan2 ( sx_yz, xx_zz ) * R2D );

}

//----------------------------------------------------------------//
void ZLQuaternion::Identity() {
	
	mS = 1.0;
	mV.mX = 0.0;
	mV.mY = 0.0;
	mV.mZ = 0.0;
}

//----------------------------------------------------------------//
void ZLQuaternion::Inverse () {
	
	real length = Length ();
	
	mS = mS / length;
	mV.mX = mV.mX / length;
	mV.mY = mV.mY / length;
	mV.mZ = mV.mZ / length;
}

//----------------------------------------------------------------//
real ZLQuaternion::Length () const {
	return sqrtf ( LengthSquared ());
}

//----------------------------------------------------------------//
real ZLQuaternion::LengthSquared () const {
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
	
	real length = Length ();
	mS /= length;
	mV.mX /= length;
	mV.mY /= length;
	mV.mZ /= length;
}

//----------------------------------------------------------------//
void ZLQuaternion::Scale ( real rhs ) {
	
	mV.mX *= rhs;
	mV.mY *= rhs;
	mV.mZ *= rhs;
	mS *= rhs;
}

//----------------------------------------------------------------//
void ZLQuaternion::Set ( const ZLAffine3D& m ) {

	real trace = m.m [ ZLAffine3D::C0_R0 ] + m.m [ ZLAffine3D::C1_R1 ] + m.m [ ZLAffine3D::C2_R2 ] + 1.0;

	if ( trace > 1.001f ) {
		
		real sqr = 2.0 * sqrtf ( trace );
		mS = 0.25f * sqr;
		mV.mX = ( m.m [ ZLAffine3D::C2_R1 ] - m.m [ ZLAffine3D::C1_R2 ] ) / sqr;
		mV.mY = ( m.m [ ZLAffine3D::C0_R1 ] - m.m [ ZLAffine3D::C2_R1 ] ) / sqr;
		mV.mZ = ( m.m [ ZLAffine3D::C1_R0 ] - m.m [ ZLAffine3D::C0_R1 ] ) / sqr;
	}
	else {
		
		if (( m.m [ ZLAffine3D::C0_R0 ] > m.m [ ZLAffine3D::C1_R1 ] ) && ( m.m [ ZLAffine3D::C0_R0 ] > m.m [ ZLAffine3D::C2_R2 ] )) {
	  
			real sqr = ( real ) sqrt( 1.0 + m.m [ ZLAffine3D::C0_R0 ] - m.m [ ZLAffine3D::C1_R1 ] - m.m [ ZLAffine3D::C2_R2 ] ) * 2; // S=4*qx
			mS = ( m.m [ ZLAffine3D::C2_R1 ] - m.m [ ZLAffine3D::C1_R2 ] ) / sqr;
			mV.mX = 0.25f * sqr;
			mV.mY = ( m.m [ ZLAffine3D::C0_R1 ] + m.m [ ZLAffine3D::C1_R0 ] ) / sqr; 
	    	mV.mZ = ( m.m [ ZLAffine3D::C0_R2 ] + m.m [ ZLAffine3D::C2_R0 ] ) / sqr; 
		}
		else if ( m.m [ ZLAffine3D::C1_R1 ] > m.m [ ZLAffine3D::C2_R2 ] ) {
	
			real sqr = ( real ) sqrt( 1.0 + m.m [ ZLAffine3D::C1_R1 ] - m.m [ ZLAffine3D::C0_R0 ] - m.m [ ZLAffine3D::C2_R2 ] ) * 2; // S=4*qy
			mS = ( m.m [ ZLAffine3D::C0_R2 ] - m.m [ ZLAffine3D::C2_R0 ] ) / sqr;
			mV.mX = ( m.m [ ZLAffine3D::C0_R1 ] + m.m [ ZLAffine3D::C1_R0 ] ) / sqr;
			mV.mY = 0.25f * sqr;
			mV.mZ = ( m.m [ ZLAffine3D::C1_R2 ] + m.m [ ZLAffine3D::C2_R1 ] ) / sqr;
		}
		else {
			
			real sqr = ( real ) sqrt( 1.0 + m.m [ ZLAffine3D::C2_R2 ] - m.m [ ZLAffine3D::C0_R0 ] - m.m [ ZLAffine3D::C1_R1 ] ) * 2; // S=4*qz
			mS = ( m.m [ ZLAffine3D::C1_R0 ] - m.m [ ZLAffine3D::C0_R1 ] ) / sqr;
			mV.mX = ( m.m [ ZLAffine3D::C0_R2 ] + m.m [ ZLAffine3D::C2_R0 ] ) / sqr;
			mV.mY = ( m.m [ ZLAffine3D::C1_R2 ] + m.m [ ZLAffine3D::C2_R1 ] ) / sqr;
			mV.mZ = 0.25f * sqr;
		}
	}
	
	this->Normalize ();
}

//----------------------------------------------------------------//
void ZLQuaternion::Set ( const ZLMatrix4x4& m ) {

	ZLAffine3D affine;
	affine.Init ( m );
	this->Set ( affine );
}

//----------------------------------------------------------------//
void ZLQuaternion::Set ( const ZLVec3D& axis, real angle ) {

	angle *= ( real )D2R;

	real s = ( real ) sin ( angle / 2.0 );

	mS = ( real ) cos ( angle / 2.0 );
	mV.mX = axis.mX * s;
	mV.mY = axis.mY * s;
	mV.mZ = axis.mZ * s;
}

//----------------------------------------------------------------//
void ZLQuaternion::Set ( real x, real y, real z ) {
	
	x *= ( real )D2R;
	y *= ( real )D2R;
	z *= ( real )D2R;
	
	real cx = Cos ( x / 2.0 );
	real cy = Cos ( y / 2.0 );
	real cz = Cos ( z / 2.0 );
	
	real sx = Sin ( x / 2.0 );
	real sy = Sin ( y / 2.0 );
	real sz = Sin ( z / 2.0 );
	
	real cycz = cy * cz;
	real sysz = sy * sz;
	real sycz = sy * cz;
	real cysz = cy * sz;

	mS		= ( cx * cycz ) - ( sx * sysz );
	mV.mX	= ( sx * sycz ) + ( cx * cysz );
	mV.mY	= ( sx * cycz ) + ( cx * sysz );
	mV.mZ	= ( cx * sycz ) - ( sx * cysz );
}

//----------------------------------------------------------------//
void ZLQuaternion::Set ( real s, real x, real y, real z ) {

	mS = s;
	mV.mX = x;
	mV.mY = y;
	mV.mZ = z;
}

//----------------------------------------------------------------//
void ZLQuaternion::Slerp ( ZLQuaternion q0, ZLQuaternion q1, real t ) {
	
	real realRound = q0.Dot ( q1 );
	
	if ( realRound > 1.0 ) {
		realRound = 1.0;
	}
	
	real angle = ( real )acos ( realRound );
	
	if ( !( angle < 0.001 && angle > -0.001 )) {
		
		q0.Scale (( real )sin ( angle * ( 1 - t )));
		q1.Scale (( real )sin ( angle * t ));
	
		q0.Add ( q1 );
		
		q0.Scale ( 1.0 / ( real )sin ( angle ));
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

	ZLQuaternion r;
	r.Set ( 0.0, loc.mX, loc.mY, loc.mZ );
	
	ZLQuaternion inv;
	inv = *this;
	inv.Inverse();
	
	inv.Multiply( r );
	inv.Multiply( *this );
	
	return inv.mV;
}
