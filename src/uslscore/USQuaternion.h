// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USQUATERNION_H
#define	USQUATERNION_H

#include <uslscore/USAffine3D.h>
#include <uslscore/USMatrix4x4.h>

//================================================================//
// USQuad
//================================================================//
class USQuaternion {
private:

public:

	USVec3D	mV;
	float	mS;

	//----------------------------------------------------------------//
	void Add ( const USQuaternion& rhs );
	
	void Init ( const USQuaternion& rhs );
	void Init ( float s, USVec3D v );
	void Init ( float x0, float y0, float x1, float y1 );

	float Dot ( const USQuaternion& rhs ) const;
	
	void Inverse ();
	
	void Identity ();
	
	float Length () const;
	float LengthSq () const;
	
	void Multiply ( const USQuaternion& rhs );
	
	void Normalize ();
	
	void FromMatrix4x4 ( const USMatrix4x4 &m );
	void ToMatrix4x4 ( USMatrix4x4 &m );

	USVec3D Rotate ( USVec3D rot );
	
	void Scale ( float rhs );
	
	static void Slerp ( USQuaternion &dest, USQuaternion q0, USQuaternion q1, float t );
	
	void Sub ( const USQuaternion& rhs );
};

#endif
