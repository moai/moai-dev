// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef VEC4D_H
#define	VEC4D_H

//================================================================//
// USMetaVec3D
//================================================================//
template < typename TYPE >
class USMetaVec4D {
public:

	TYPE	mX;
	TYPE	mY;
	TYPE	mZ;
	TYPE	mW;

	//----------------------------------------------------------------//
	void Lerp ( const USMetaVec4D& vec, TYPE time ) {

		this->mX = this->mX + (( vec.mX - this->mX ) * time );
		this->mY = this->mY + (( vec.mY - this->mY ) * time );
		this->mZ = this->mZ + (( vec.mZ - this->mZ ) * time );
		this->mW = this->mW + (( vec.mW - this->mW ) * time );
	}

	//----------------------------------------------------------------//
	USMetaVec4D () {
	}

	//----------------------------------------------------------------//
	USMetaVec4D ( float x, float y, float z, float w ) :
		mX ( x ),
		mY ( y ),
		mZ ( z ),
		mW ( w ) {
	}

	//----------------------------------------------------------------//
	~USMetaVec4D () {
	}
};

typedef USMetaVec4D < int > USIntVec4D;
typedef USMetaVec4D < float > USVec4D;
typedef USMetaVec4D < double > USVec4D64;


#endif
