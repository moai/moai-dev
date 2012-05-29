
#include "pch.h"
#include <uslscore/USQuaternion.h>

//================================================================//
// USQuaternion
//================================================================//

//----------------------------------------------------------------//
void USQuaternion::Add ( const USQuaternion& rhs ) {
	
	mS += rhs.mS;
	mV.mX += rhs.mV.mX;
	mV.mY += rhs.mV.mY;
	mV.mZ += rhs.mV.mZ;
}

//----------------------------------------------------------------//
float USQuaternion::Dot ( const USQuaternion& rhs ) const {
	return ( mS * rhs.mS ) + mV.Dot ( rhs.mV );
}

//----------------------------------------------------------------//
void USQuaternion::Get ( USAffine3D& m ) const {
	
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

	m.m [ USAffine3D::C0_R0 ] = 1.0f - ( yy + zz );
	m.m [ USAffine3D::C0_R1 ] = xy - wz;
	m.m [ USAffine3D::C0_R2 ] = xz + wy;

	m.m [ USAffine3D::C1_R0 ] = xy + wz;
	m.m [ USAffine3D::C1_R1 ] = 1.0f - ( xx + zz );
	m.m [ USAffine3D::C1_R2 ] = yz - wx;

	m.m [ USAffine3D::C2_R0 ] = xz - wy;
	m.m [ USAffine3D::C2_R1 ] = yz + wx;
	m.m [ USAffine3D::C2_R2 ] = 1.0f - ( xx + yy );

	m.m [ USAffine3D::C3_R0 ] = 0.0f;
	m.m [ USAffine3D::C3_R1 ] = 0.0f;
	m.m [ USAffine3D::C3_R2 ] = 0.0f;
	
	//m.m [ USAffine3D::C0_R0 ] = 1.0f - 2.0f * ( mV.mY * mV.mY + mV.mZ * mV.mZ );
	//m.m [ USAffine3D::C0_R1 ] = 2.0f * ( mV.mX * mV.mY - mS * mV.mZ );
	//m.m [ USAffine3D::C0_R2 ] = 2.0f * ( mV.mX * mV.mZ + mS * mV.mY );
	//
	//m.m [ USAffine3D::C1_R0 ] = 2.0f * ( mV.mX * mV.mY + mS * mV.mZ );
	//m.m [ USAffine3D::C1_R1 ] = 1.0f - 2.0f * ( mV.mX * mV.mX + mV.mZ * mV.mZ );
	//m.m [ USAffine3D::C1_R2 ] = 2.0f * ( mV.mY * mV.mZ - mS * mV.mX );
	//
	//m.m [ USAffine3D::C2_R0 ] = 2.0f * ( mV.mX * mV.mZ - mS * mV.mY );
	//m.m [ USAffine3D::C2_R1 ] = 2.0f * ( mV.mY * mV.mZ + mS * mV.mX );
	//m.m [ USAffine3D::C2_R2 ] = 1.0f - 2.0f * ( mV.mX * mV.mX + mV.mY * mV.mY );
	//
	//m.m [ USAffine3D::C3_R0 ] = 0.0f;
	//m.m [ USAffine3D::C3_R1 ] = 0.0f;
	//m.m [ USAffine3D::C3_R2 ] = 0.0f;
}

//----------------------------------------------------------------//
void USQuaternion::Get ( USMatrix4x4& m ) const {
	
	USAffine3D affine;
	this->Get ( affine );
	m.Init ( affine );
}

//----------------------------------------------------------------//
void USQuaternion::Get ( USVec3D& axis, float& angle ) const {

	float sqrLen = ( mV.mX * mV.mX ) + ( mV.mY * mV.mY ) + (mV. mZ * mV.mZ );

	if ( sqrLen > 0.0f ) {
		angle = 2.0f * ( float ) acos ( mS );
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

void USQuaternion::Get ( float& x, float& y, float& z ) const {
	
	x = ( float ) atan2 ( 2.0f * ( mS * mV.mX + mV.mY * mV.mZ ), 1.0f - 2.0f * ( mV.mX * mV.mX + mV.mY * mV.mY ));
	y = asinf ( 2.0f * ( mS * mV.mY + mV.mX * mV.mZ ));
	z = ( float ) atan2 ( 2.0f * ( mS * mV.mZ + mV.mX * mV.mY ), 1.0f - 2.0f * ( mV.mY * mV.mY + mV.mZ * mV.mZ ));
}

//----------------------------------------------------------------//
void USQuaternion::Identity() {
	
	mS = 1.0f;
	mV.mX = 0.0f;
	mV.mY = 0.0f;
	mV.mZ = 0.0f;
}

//----------------------------------------------------------------//
void USQuaternion::Inverse () {
	
	float length = Length ();
	
	mS = mS / length;
	mV.mX = mV.mX / length;
	mV.mY = mV.mY / length;
	mV.mZ = mV.mZ / length;
}

//----------------------------------------------------------------//
float USQuaternion::Length () const {
	return sqrtf ( LengthSquared ());
}

//----------------------------------------------------------------//
float USQuaternion::LengthSquared () const {
	return  ( mS * mS ) + ( mV.mX * mV.mX ) + ( mV.mY * mV.mY ) + ( mV.mZ * mV.mZ );
}

//----------------------------------------------------------------//
void USQuaternion::Multiply ( const USQuaternion& rhs ) {

	USVec3D cross;
	cross.Cross ( mV, rhs.mV );
	
	USVec3D resultVec;
	resultVec.Init ( rhs.mV );
	resultVec.Scale ( mS );
	
	USVec3D scaledVec;
	scaledVec.Init ( mV );
	scaledVec.Scale ( rhs.mS );
	
	resultVec.Add ( scaledVec );
	resultVec.Sub ( cross );
	
	mS = mS * rhs.mS - mV.Dot ( rhs.mV );
	mV.Init ( resultVec );
	
}

//----------------------------------------------------------------//
void USQuaternion::Normalize() {
	
	float length = Length ();
	mS /= length;
	mV.mX /= length;
	mV.mY /= length;
	mV.mZ /= length;
}

//----------------------------------------------------------------//
USVec3D USQuaternion::Rotate ( USVec3D rot ) const {

	USQuaternion r;
	r.Set ( 0.0f, rot.mX, rot.mY, rot.mZ );
	
	USQuaternion inv;
	inv = *this;
	inv.Inverse();
	
	inv.Multiply( r );
	inv.Multiply( *this );
	
	return inv.mV;
}

//----------------------------------------------------------------//
void USQuaternion::Scale ( float rhs ) {
	
	mV.mX *= rhs;
	mV.mY *= rhs;
	mV.mZ *= rhs;
	mS *= rhs;
}

//----------------------------------------------------------------//
void USQuaternion::Set ( const USAffine3D& m ) {

	float trace = m.m [ USAffine3D::C0_R0 ] + m.m [ USAffine3D::C1_R1 ] + m.m [ USAffine3D::C2_R2 ] + 1.0f;

	if ( trace > 1.001f ) {
		
		float sqr = 2.0f * sqrtf ( trace );
		mS = 0.25f * sqr;
		mV.mX = ( m.m [ USAffine3D::C2_R1 ] - m.m [ USAffine3D::C1_R2 ] ) / sqr;
		mV.mY = ( m.m [ USAffine3D::C0_R1 ] - m.m [ USAffine3D::C2_R1 ] ) / sqr;
		mV.mZ = ( m.m [ USAffine3D::C1_R0 ] - m.m [ USAffine3D::C0_R1 ] ) / sqr;
	}
	else {
		
		if (( m.m [ USAffine3D::C0_R0 ] > m.m [ USAffine3D::C1_R1 ] ) && ( m.m [ USAffine3D::C0_R0 ] > m.m [ USAffine3D::C2_R2 ] )) {
	  
			float sqr = ( float ) sqrt( 1.0 + m.m [ USAffine3D::C0_R0 ] - m.m [ USAffine3D::C1_R1 ] - m.m [ USAffine3D::C2_R2 ] ) * 2; // S=4*qx
			mS = ( m.m [ USAffine3D::C2_R1 ] - m.m [ USAffine3D::C1_R2 ] ) / sqr;
			mV.mX = 0.25f * sqr;
			mV.mY = ( m.m [ USAffine3D::C0_R1 ] + m.m [ USAffine3D::C1_R0 ] ) / sqr; 
	    	mV.mZ = ( m.m [ USAffine3D::C0_R2 ] + m.m [ USAffine3D::C2_R0 ] ) / sqr; 
		}
		else if ( m.m [ USAffine3D::C1_R1 ] > m.m [ USAffine3D::C2_R2 ] ) {
	
			float sqr = ( float ) sqrt( 1.0 + m.m [ USAffine3D::C1_R1 ] - m.m [ USAffine3D::C0_R0 ] - m.m [ USAffine3D::C2_R2 ] ) * 2; // S=4*qy
			mS = ( m.m [ USAffine3D::C0_R2 ] - m.m [ USAffine3D::C2_R0 ] ) / sqr;
			mV.mX = ( m.m [ USAffine3D::C0_R1 ] + m.m [ USAffine3D::C1_R0 ] ) / sqr;
			mV.mY = 0.25f * sqr;
			mV.mZ = ( m.m [ USAffine3D::C1_R2 ] + m.m [ USAffine3D::C2_R1 ] ) / sqr;
		}
		else {
			
			float sqr = ( float ) sqrt( 1.0 + m.m [ USAffine3D::C2_R2 ] - m.m [ USAffine3D::C0_R0 ] - m.m [ USAffine3D::C1_R1 ] ) * 2; // S=4*qz
			mS = ( m.m [ USAffine3D::C1_R0 ] - m.m [ USAffine3D::C0_R1 ] ) / sqr;
			mV.mX = ( m.m [ USAffine3D::C0_R2 ] + m.m [ USAffine3D::C2_R0 ] ) / sqr;
			mV.mY = ( m.m [ USAffine3D::C1_R2 ] + m.m [ USAffine3D::C2_R1 ] ) / sqr;
			mV.mZ = 0.25f * sqr;
		}
	}
	
	this->Normalize ();
	
	//float trace;
	//float root;

	//trace = mtx.m[USMatrix3D::C0_R0] + mtx.m[USMatrix3D::C1_R1] + mtx.m[USMatrix3D::C2_R2];

	//if ( trace > 0.0f ) {
	//	root = Sqrt ( trace + 1.0f );
	//	mS = 0.5f * root;
	//	root = 0.5f / root;
	//	mX = ( mtx.m[USMatrix3D::C1_R2] - mtx.m[USMatrix3D::C2_R1] ) * root;
	//	mY = ( mtx.m[USMatrix3D::C2_R0] - mtx.m[USMatrix3D::C0_R2] ) * root;
	//	mZ = ( mtx.m[USMatrix3D::C0_R1] - mtx.m[USMatrix3D::C1_R0] ) * root;
	//}
	//else {
	//	u32 next[3] = { 1, 2, 0 };

	//	u32 i = 0;
	//	if ( mtx.m[USMatrix3D::C1_R1] > mtx.m[USMatrix3D::C0_R0] ) i = 1;
	//	if ( mtx.m[USMatrix3D::C2_R2] > mtx.GetElement ( i, i )) i = 2;

	//	u32 j = next[i];
	//	u32 k = next[j];

	//	root = Sqrt (( mtx.GetElement ( i, i ) - ( mtx.GetElement ( j, j ) + mtx.GetElement ( k, k ) )) + 1.0f );

	//	float* quat[3] = { &mX, &mY, &mZ };

	//	*quat[i] = root * 0.5f;

	//	if ( root != 0.0f ) root = 0.5f / root;

	//	*quat[j] = ( mtx.GetElement ( i, j ) + mtx.GetElement ( j, i )) * root;
	//	*quat[k] = ( mtx.GetElement ( i, k ) + mtx.GetElement ( k, i )) * root;
	//	mS = ( mtx.GetElement ( j, k ) - mtx.GetElement ( k, j )) * root;
	//}
}

//----------------------------------------------------------------//
void USQuaternion::Set ( const USMatrix4x4& m ) {

	USAffine3D affine;
	affine.Init ( m );
	this->Set ( affine );
}

//----------------------------------------------------------------//
void USQuaternion::Set ( const USVec3D& axis, float angle ) {

	float s = ( float ) sin ( angle / 2.0f );

	mS = ( float ) cos ( angle / 2.0f );
	mV.mX = axis.mX * s;
	mV.mY = axis.mY * s;
	mV.mZ = axis.mZ * s;
}

//----------------------------------------------------------------//
void USQuaternion::Set ( float x, float y, float z ) {
	
	/*float cx = Cos ( x / 2.0f );
	float cy = Cos ( y / 2.0f );
	float cz = Cos ( z / 2.0f );

	float sx = Sin ( x / 2.0f );
	float sy = Sin ( y / 2.0f );
	float sz = Sin ( z / 2.0f );

	float cxcy = cx * cy;
	float sxsy = sx * sy;
	float sxcy = sx * cy;
	float cxsy = cx * sy;

	mS		= ( cz * cxcy ) + ( sz * sxsy );
	mV.mX	= ( sz * cxcy ) - ( cz * sxsy );
	mV.mY	= ( cz * sxcy ) + ( sz * cxsy );
	mV.mZ	= ( cz * cxsy ) - ( sz * sxcy );*/
	
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
	
	mS		= ( cx * cycz ) + ( sx * sysz );
	mV.mX	= ( sx * cycz ) - ( cx * sysz );
	mV.mY	= ( cx * sycz ) + ( sx * cysz );
	mV.mZ	= ( cx * cysz ) - ( sx * sycz );
}

//----------------------------------------------------------------//
void USQuaternion::Set ( float s, float x, float y, float z ) {

	mS = s;
	mV.mX = x;
	mV.mY = y;
	mV.mZ = z;
}

//----------------------------------------------------------------//
void USQuaternion::Slerp ( USQuaternion q0, USQuaternion q1, float t ) {
	
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
void USQuaternion::Sub ( const USQuaternion& rhs ) {
	
	mS -= rhs.mS;
	mV.mX -= rhs.mV.mX;
	mV.mY -= rhs.mV.mY;
	mV.mZ -= rhs.mV.mZ;
}

