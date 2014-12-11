// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLVEC4D_H
#define	ZLVEC4D_H

//================================================================//
// ZLMetaVec4D
//================================================================//
template < typename TYPE >
class ZLMetaVec4D {
public:

	TYPE	mX;
	TYPE	mY;
	TYPE	mZ;
	TYPE	mW;

	//----------------------------------------------------------------//
	void Init ( TYPE x, TYPE y, TYPE z, TYPE w ) {
		mX = x;
		mY = y;
		mZ = z;
		mW = w;
	}

	//----------------------------------------------------------------//
	void Lerp ( const ZLMetaVec4D& vec, TYPE time ) {

		this->mX = this->mX + (( vec.mX - this->mX ) * time );
		this->mY = this->mY + (( vec.mY - this->mY ) * time );
		this->mZ = this->mZ + (( vec.mZ - this->mZ ) * time );
		this->mW = this->mW + (( vec.mW - this->mW ) * time );
	}

	//----------------------------------------------------------------//
	void Scale ( TYPE s ) {
	
		this->mX *= s;
		this->mY *= s;
		this->mZ *= s;
		this->mW *= s;
	}

	//----------------------------------------------------------------//
	ZLMetaVec4D () {
	}

	//----------------------------------------------------------------//
	ZLMetaVec4D ( float x, float y, float z, float w ) :
		mX ( x ),
		mY ( y ),
		mZ ( z ),
		mW ( w ) {
	}

	//----------------------------------------------------------------//
	~ZLMetaVec4D () {
	}
};

typedef ZLMetaVec4D < int > ZLIntVec4D;
typedef ZLMetaVec4D < float > ZLVec4D;
typedef ZLMetaVec4D < double > ZLVec4D64;


#endif
