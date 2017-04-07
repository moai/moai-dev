
#include "pch.h"
#include <zl-util/ZLQuaternion.h>

//================================================================//
// ZLQuaternionStruct
//================================================================//

//----------------------------------------------------------------//
void ZLQuaternionStruct::Add ( const ZLQuaternionStruct& rhs ) {
	
	mS += rhs.mS;
	mV.mX += rhs.mV.mX;
	mV.mY += rhs.mV.mY;
	mV.mZ += rhs.mV.mZ;
}

//----------------------------------------------------------------//
float ZLQuaternionStruct::Dot ( const ZLQuaternionStruct& rhs ) const {
	return ( mS * rhs.mS ) + mV.Dot ( rhs.mV );
}

//----------------------------------------------------------------//
void ZLQuaternionStruct::Get ( ZLAffine3D& m ) const {
	
	ZLMatrix3x3 affine;
	this->Get ( affine );
	m = ZLAffine3D ( affine );
}

//----------------------------------------------------------------//
void ZLQuaternionStruct::Get ( ZLMatrix3x3& m ) const {
	
	float x2 = mV.mX + mV.mX;
	float y2 = mV.mY + mV.mY;
	float z2 = mV.mZ + mV.mZ;
	float xx = mV.mX * x2;
	float xy = mV.mX * y2;
	float xz = mV.mX * z2;
	float yy = mV.mY * y2;
	float yz = mV.mY * z2;
	float zz = mV.mZ * z2;
	float wx = mS * x2;
	float wy = mS * y2;
	float wz = mS * z2;

	m.m [ ZLMatrix3x3::C0_R0 ] = 1.0f - ( yy + zz );
	m.m [ ZLMatrix3x3::C0_R1 ] = xy - wz;
	m.m [ ZLMatrix3x3::C0_R2 ] = xz + wy;

	m.m [ ZLMatrix3x3::C1_R0 ] = xy + wz;
	m.m [ ZLMatrix3x3::C1_R1 ] = 1.0f - ( xx + zz );
	m.m [ ZLMatrix3x3::C1_R2 ] = yz - wx;

	m.m [ ZLMatrix3x3::C2_R0 ] = xz - wy;
	m.m [ ZLMatrix3x3::C2_R1 ] = yz + wx;
	m.m [ ZLMatrix3x3::C2_R2 ] = 1.0f - ( xx + yy );
	
	m.Transpose ();
}

//----------------------------------------------------------------//
void ZLQuaternionStruct::Get ( ZLMatrix4x4& m ) const {
	
	ZLMatrix3x3 mtx3x3;
	this->Get ( mtx3x3 );
	m = ZLMatrix4x4 ( mtx3x3 );
}

//----------------------------------------------------------------//
void ZLQuaternionStruct::Get ( ZLVec3D& axis, float& angle ) const {

	float sqrLen = ( mV.mX * mV.mX ) + ( mV.mY * mV.mY ) + ( mV.mZ * mV.mZ );

	if ( sqrLen > 0.0f ) {
		angle = 2.0f * ( float )( acos ( mS ) * R2D );
		sqrLen = 1.0f / ( float )sqrt ( sqrLen );
		axis.mX = mV.mX * sqrLen;
		axis.mY = mV.mY * sqrLen;
		axis.mZ = mV.mZ * sqrLen;
	}
	else {
		angle = 0.0f;
		axis.mX = 0.0f;
		axis.mY = 1.0f;
		axis.mZ = 0.0f;
	}
}

//----------------------------------------------------------------//
void ZLQuaternionStruct::Get ( float& x, float& y, float& z ) const {

	float sz_xy = 2.0f * ( mS * mV.mZ + mV.mX * mV.mY );

	if ( sz_xy > 1.0f ) {
		sz_xy = 1.0f;
	}
	else if ( sz_xy < -1.0f ) {
		sz_xy = -1.0f;
	}

	float sx_yz = 2.0f * ( mS * mV.mX - mV.mY * mV.mZ );
	float xx_zz = 1.0f - 2.0f * ( mV.mX * mV.mX + mV.mZ * mV.mZ );

	float sy_xz = 2.0f * ( mS * mV.mY - mV.mX * mV.mZ );
	float yy_zz = 1.0f - 2.0f * ( mV.mY * mV.mY + mV.mZ * mV.mZ );

	//round values to prevent floating errors affecting results
	if ( sx_yz < EPSILON && sx_yz > - EPSILON ) {
		sx_yz = 0.0f;
	}
	if ( xx_zz < EPSILON && xx_zz > - EPSILON ) {
		xx_zz = 0.0f;
	}

	if ( sy_xz < EPSILON && sy_xz > - EPSILON ) {
		sy_xz = 0.0f;
	}
	if ( yy_zz < EPSILON && yy_zz > - EPSILON ) {
		yy_zz = 0.0f;
	}

	x = ( float )( atan2 ( sy_xz, yy_zz ) * R2D );
	y = ( float )( asin ( sz_xy ) * R2D );
	z = ( float )( atan2 ( sx_yz, xx_zz ) * R2D );

}

//----------------------------------------------------------------//
void ZLQuaternionStruct::Inverse () {
	
	float length = Length ();
	
	mS = mS / length;
	mV.mX = mV.mX / length;
	mV.mY = mV.mY / length;
	mV.mZ = mV.mZ / length;
}

//----------------------------------------------------------------//
float ZLQuaternionStruct::Length () const {
	return sqrtf ( LengthSquared ());
}

//----------------------------------------------------------------//
float ZLQuaternionStruct::LengthSquared () const {
	return  ( mS * mS ) + ( mV.mX * mV.mX ) + ( mV.mY * mV.mY ) + ( mV.mZ * mV.mZ );
}

//----------------------------------------------------------------//
void ZLQuaternionStruct::Multiply ( const ZLQuaternionStruct& rhs ) {

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
void ZLQuaternionStruct::Normalize() {
	
	float length = Length ();
	
	mS /= length;
	mV.mX /= length;
	mV.mY /= length;
	mV.mZ /= length;
}

//----------------------------------------------------------------//
void ZLQuaternionStruct::Scale ( float rhs ) {
	
	mV.mX *= rhs;
	mV.mY *= rhs;
	mV.mZ *= rhs;
	mS *= rhs;
}

//----------------------------------------------------------------//
void ZLQuaternionStruct::Slerp ( ZLQuaternionStruct q0, ZLQuaternionStruct q1, float t ) {
	
	float floatRound = q0.Dot ( q1 );
	
	if ( floatRound > 1.0f ) {
		floatRound = 1.0f;
	}
	
	float angle = ( float )acos ( floatRound );
	
	if ( !( angle < 0.001 && angle > -0.001 )) {
		
		q0.Scale (( float )sin ( angle * ( 1 - t )));
		q1.Scale (( float )sin ( angle * t ));
	
		q0.Add ( q1 );
		
		q0.Scale ( 1.0f / ( float )sin ( angle ));
	}
	
	*this = q0;
}

//----------------------------------------------------------------//
void ZLQuaternionStruct::Sub ( const ZLQuaternionStruct& rhs ) {
	
	mS -= rhs.mS;
	mV.mX -= rhs.mV.mX;
	mV.mY -= rhs.mV.mY;
	mV.mZ -= rhs.mV.mZ;
}

//----------------------------------------------------------------//
ZLVec3D ZLQuaternionStruct::Transform ( ZLVec3D loc ) const {

	ZLQuaternion r ( 0.0f, loc.mX, loc.mY, loc.mZ );
	
	ZLQuaternionStruct inv;
	inv = *this;
	inv.Inverse();
	
	inv.Multiply( r );
	inv.Multiply( *this );
	
	return inv.mV;
}

//================================================================//
// ZLQuaternion
//================================================================//

const ZLQuaternion ZLQuaternion::IDENT = ZLQuaternion ( 1.0f, 0.0f, 0.0f, 0.0f );

//----------------------------------------------------------------//
ZLQuaternion::ZLQuaternion () {
}

//----------------------------------------------------------------//
ZLQuaternion::ZLQuaternion ( const ZLQuaternionStruct& quat ) {

	*( ZLQuaternionStruct* )this = quat;
}

//----------------------------------------------------------------//
ZLQuaternion::ZLQuaternion ( const ZLAffine3D& m ) {

	*this = ZLQuaternion ( ZLMatrix3x3 ( m ));
}

//----------------------------------------------------------------//
ZLQuaternion::ZLQuaternion ( ZLMatrix3x3 m ) {

	m.Transpose ();

	float trace = m.m [ ZLMatrix3x3::C0_R0 ] + m.m [ ZLMatrix3x3::C1_R1 ] + m.m [ ZLMatrix3x3::C2_R2 ] + 1.0f;

	if ( trace > 1.001f ) {
		
		float sqr = 2.0f * sqrtf ( trace );
		mS = 0.25f * sqr;
		mV.mX = ( m.m [ ZLMatrix3x3::C2_R1 ] - m.m [ ZLMatrix3x3::C1_R2 ] ) / sqr;
		mV.mY = ( m.m [ ZLMatrix3x3::C0_R1 ] - m.m [ ZLMatrix3x3::C2_R1 ] ) / sqr;
		mV.mZ = ( m.m [ ZLMatrix3x3::C1_R0 ] - m.m [ ZLMatrix3x3::C0_R1 ] ) / sqr;
	}
	else {
		
		if (( m.m [ ZLMatrix3x3::C0_R0 ] > m.m [ ZLMatrix3x3::C1_R1 ] ) && ( m.m [ ZLMatrix3x3::C0_R0 ] > m.m [ ZLMatrix3x3::C2_R2 ] )) {
	  
			float sqr = ( float ) sqrt( 1.0 + m.m [ ZLMatrix3x3::C0_R0 ] - m.m [ ZLMatrix3x3::C1_R1 ] - m.m [ ZLMatrix3x3::C2_R2 ] ) * 2; // S=4*qx
			mS = ( m.m [ ZLMatrix3x3::C2_R1 ] - m.m [ ZLMatrix3x3::C1_R2 ] ) / sqr;
			mV.mX = 0.25f * sqr;
			mV.mY = ( m.m [ ZLMatrix3x3::C0_R1 ] + m.m [ ZLMatrix3x3::C1_R0 ] ) / sqr;
	    	mV.mZ = ( m.m [ ZLMatrix3x3::C0_R2 ] + m.m [ ZLMatrix3x3::C2_R0 ] ) / sqr;
		}
		else if ( m.m [ ZLMatrix3x3::C1_R1 ] > m.m [ ZLMatrix3x3::C2_R2 ] ) {
	
			float sqr = ( float ) sqrt( 1.0 + m.m [ ZLMatrix3x3::C1_R1 ] - m.m [ ZLMatrix3x3::C0_R0 ] - m.m [ ZLMatrix3x3::C2_R2 ] ) * 2; // S=4*qy
			mS = ( m.m [ ZLMatrix3x3::C0_R2 ] - m.m [ ZLMatrix3x3::C2_R0 ] ) / sqr;
			mV.mX = ( m.m [ ZLMatrix3x3::C0_R1 ] + m.m [ ZLMatrix3x3::C1_R0 ] ) / sqr;
			mV.mY = 0.25f * sqr;
			mV.mZ = ( m.m [ ZLMatrix3x3::C1_R2 ] + m.m [ ZLMatrix3x3::C2_R1 ] ) / sqr;
		}
		else {
			
			float sqr = ( float ) sqrt( 1.0 + m.m [ ZLMatrix3x3::C2_R2 ] - m.m [ ZLMatrix3x3::C0_R0 ] - m.m [ ZLMatrix3x3::C1_R1 ] ) * 2; // S=4*qz
			mS = ( m.m [ ZLMatrix3x3::C1_R0 ] - m.m [ ZLMatrix3x3::C0_R1 ] ) / sqr;
			mV.mX = ( m.m [ ZLMatrix3x3::C0_R2 ] + m.m [ ZLMatrix3x3::C2_R0 ] ) / sqr;
			mV.mY = ( m.m [ ZLMatrix3x3::C1_R2 ] + m.m [ ZLMatrix3x3::C2_R1 ] ) / sqr;
			mV.mZ = 0.25f * sqr;
		}
	}
	
	this->Normalize ();
}

//----------------------------------------------------------------//
ZLQuaternion::ZLQuaternion ( const ZLMatrix4x4& m ) {

	*this = ZLQuaternion ( ZLMatrix3x3 ( m ));
}

//----------------------------------------------------------------//
ZLQuaternion::ZLQuaternion ( const ZLVec3D& axis, float angle ) {

	angle *= ( float )D2R;

	float s = ( float ) sin ( angle / 2.0f );

	mS = ( float ) cos ( angle / 2.0f );
	mV.mX = axis.mX * s;
	mV.mY = axis.mY * s;
	mV.mZ = axis.mZ * s;
}

//----------------------------------------------------------------//
ZLQuaternion::ZLQuaternion ( float x, float y, float z ) {
	
	x *= ( float )D2R;
	y *= ( float )D2R;
	z *= ( float )D2R;
	
	float cx = Cos ( x / 2.0f );
	float cy = Cos ( y / 2.0f );
	float cz = Cos ( z / 2.0f );
	
	float sx = Sin ( x / 2.0f );
	float sy = Sin ( y / 2.0f );
	float sz = Sin ( z / 2.0f );
	
	float cycz = cy * cz;
	float sysz = sy * sz;
	float sycz = sy * cz;
	float cysz = cy * sz;

	mS		= ( cx * cycz ) - ( sx * sysz );
	mV.mX	= ( sx * sycz ) + ( cx * cysz );
	mV.mY	= ( sx * cycz ) + ( cx * sysz );
	mV.mZ	= ( cx * sycz ) - ( sx * cysz );
}

//----------------------------------------------------------------//
ZLQuaternion::ZLQuaternion ( float s, float x, float y, float z ) {

	mS		= s;
	mV.mX	= x;
	mV.mY	= y;
	mV.mZ	= z;
}
