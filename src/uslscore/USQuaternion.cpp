
#include "pch.h"
#include <USQuaternion.h>

void USQuaternion::Add ( const USQuaternion& rhs ) {
	
	mS += rhs.mS;
	mV.mX += rhs.mV.mX;
	mV.mY += rhs.mV.mY;
	mV.mZ += rhs.mV.mZ;
}

float USQuaternion::Dot ( const USQuaternion& rhs ) const {
	return mS * rhs.mS + mV.Dot ( rhs.mV );
}

void USQuaternion::Identity() {
	
	mS = 1.0f;
	mV.mX = 0.0f;
	mV.mY = 0.0f;
	mV.mZ = 0.0f;
}

void USQuaternion::Init ( float s, USVec3D v ) {

	mS = s;
	mV.mX = v.mX;
	mV.mY = v.mY;
	mV.mZ = v.mZ;
}

void USQuaternion::Init ( float f, float x, float y, float z ) {

	mS = f;
	mV.mX = x;
	mV.mY = y;
	mV.mZ = z;
}

void USQuaternion::Init ( const USQuaternion& rhs ) {
	
	mS = rhs.mS;
	mV.mX = rhs.mV.mX;
	mV.mY = rhs.mV.mY;
	mV.mZ = rhs.mV.mZ;
}

void USQuaternion::Inverse() {
	
	float length = Length ();
	
	mS = mS / length;
	mV.mX = mV.mX / length;
	mV.mY = mV.mY / length;
	mV.mZ = mV.mZ / length;
	
}

void USQuaternion::FromMatrix4x4 ( const USMatrix4x4 &m )
{
	float trace = m.m [ USMatrix4x4::C0_R0 ] + m.m [ USMatrix4x4::C1_R1 ] + m.m [ USMatrix4x4::C2_R2 ] + 1.0f;

	if ( trace > 1.001f ) {
		
		float sqr = 2.0f * sqrtf ( trace );
		mS = 0.25f * sqr;
		mV.mX = ( m.m [ USMatrix4x4::C2_R1 ] - m.m [ USMatrix4x4::C1_R2 ] ) / sqr;
		mV.mY = ( m.m [ USMatrix4x4::C0_R1 ] - m.m [ USMatrix4x4::C2_R1 ] ) / sqr;
		mV.mZ = ( m.m [ USMatrix4x4::C1_R0 ] - m.m [ USMatrix4x4::C0_R1 ] ) / sqr;
	}
	else {
		
		if (( m.m [ USMatrix4x4::C0_R0 ] > m.m [ USMatrix4x4::C1_R1 ] ) && ( m.m [ USMatrix4x4::C0_R0 ] > m.m [ USMatrix4x4::C2_R2 ] )) {
	  
			float sqr = ( float ) sqrt( 1.0 + m.m [ USMatrix4x4::C0_R0 ] - m.m [ USMatrix4x4::C1_R1 ] - m.m [ USMatrix4x4::C2_R2 ] ) * 2; // S=4*qx
			mS = ( m.m [ USMatrix4x4::C2_R1 ] - m.m [ USMatrix4x4::C1_R2 ] ) / sqr;
			mV.mX = 0.25f * sqr;
			mV.mY = ( m.m [ USMatrix4x4::C0_R1 ] + m.m [ USMatrix4x4::C1_R0 ] ) / sqr; 
	    	mV.mZ = ( m.m [ USMatrix4x4::C0_R2 ] + m.m [ USMatrix4x4::C2_R0 ] ) / sqr; 
		}
		else if ( m.m [ USMatrix4x4::C1_R1 ] > m.m [ USMatrix4x4::C2_R2 ] ) {
	
			float sqr = ( float ) sqrt( 1.0 + m.m [ USMatrix4x4::C1_R1 ] - m.m [ USMatrix4x4::C0_R0 ] - m.m [ USMatrix4x4::C2_R2 ] ) * 2; // S=4*qy
			mS = ( m.m [ USMatrix4x4::C0_R2 ] - m.m [ USMatrix4x4::C2_R0 ] ) / sqr;
			mV.mX = ( m.m [ USMatrix4x4::C0_R1 ] + m.m [ USMatrix4x4::C1_R0 ] ) / sqr;
			mV.mY = 0.25f * sqr;
			mV.mZ = ( m.m [ USMatrix4x4::C1_R2 ] + m.m [ USMatrix4x4::C2_R1 ] ) / sqr;
		}
		else {
			
			float sqr = ( float ) sqrt( 1.0 + m.m [ USMatrix4x4::C2_R2 ] - m.m [ USMatrix4x4::C0_R0 ] - m.m [ USMatrix4x4::C1_R1 ] ) * 2; // S=4*qz
			mS = ( m.m [ USMatrix4x4::C1_R0 ] - m.m [ USMatrix4x4::C0_R1 ] ) / sqr;
			mV.mX = ( m.m [ USMatrix4x4::C0_R2 ] + m.m [ USMatrix4x4::C2_R0 ] ) / sqr;
			mV.mY = ( m.m [ USMatrix4x4::C1_R2 ] + m.m [ USMatrix4x4::C2_R1 ] ) / sqr;
			mV.mZ = 0.25f * sqr;
		}
	}
	
	Normalize ();
}

void USQuaternion::ToMatrix4x4 ( USMatrix4x4 &m ) {
	
	m.m [ USMatrix4x4::C0_R0 ] = 1.0f - 2.0f * ( mV.mY * mV.mY + mV.mZ * mV.mZ );
	m.m [ USMatrix4x4::C0_R1 ] = 2.0f * ( mV.mX * mV.mY - mS * mV.mZ);
	m.m [ USMatrix4x4::C0_R2 ] = 2.0f * ( mV.mX * mV.mZ + mS * mV.mY);
	
	m.m [ USMatrix4x4::C1_R0 ] = 2.0f * ( mV.mX * mV.mY + mS * mV.mZ);
	m.m [ USMatrix4x4::C1_R1 ] = 1.0f - 2.0f * ( mV.mX * mV.mX + mV.mZ * mV.mZ );
	m.m [ USMatrix4x4::C1_R2 ] = 2.0f * ( mV.mY * mV.mZ - mS * mV.mX );
	
	m.m [ USMatrix4x4::C2_R0 ] = 2.0f * ( mV.mX * mV.mZ - mS * mV.mY );
	m.m [ USMatrix4x4::C2_R1 ] = 2.0f * ( mV.mY * mV.mZ + mS * mV.mX );
	m.m [ USMatrix4x4::C2_R2 ] = 1.0f - 2.0f * ( mV.mX * mV.mX + mV.mY * mV.mY );
	
	m.m [ USMatrix4x4::C3_R0 ] = 0.0f;
	m.m [ USMatrix4x4::C3_R1 ] = 0.0f;
	m.m [ USMatrix4x4::C3_R2 ] = 0.0f;
	m.m [ USMatrix4x4::C3_R3 ] = 1.0f;
}

float USQuaternion::Length () const {
	return sqrtf ( LengthSq () );
}

float USQuaternion::LengthSq () const {
	return mS * mS + mV.mX * mV.mX + mV.mY * mV.mY + mV.mZ * mV.mZ;
}

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

void USQuaternion::Normalize() {
	
	float length = Length ();
	mS /= length;
	mV.mX /= length;
	mV.mY /= length;
	mV.mZ /= length;
}

USVec3D USQuaternion::Rotate ( USVec3D pos ) {
	USQuaternion r;
	r.Init ( 0.0f, pos );
	
	USQuaternion inv;
	inv.Init ( *this );
	inv.Inverse();
	
	inv.Multiply( r );
	inv.Multiply( *this );
	
	return inv.mV;
}

void USQuaternion::Scale ( float rhs ) {
	
	mS *= rhs;
	mV.mX *= rhs;
	mV.mY *= rhs;
	mV.mZ *= rhs;
}

void USQuaternion::Sub ( const USQuaternion& rhs ) {
	
	mV.mX -= rhs.mV.mX;
	mV.mY -= rhs.mV.mY;
	mV.mZ -= rhs.mV.mZ;
	mS -= rhs.mS;
}
