// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLVEC4D_H
#define	ZLVEC4D_H

#include <zl-util/ZLVec3D.h>

//================================================================//
// ZLMetaVec4D
//================================================================//
template < typename TYPE >
class ZLMetaVec4D :
	public ZLMetaVec3D < TYPE > {
public:

	static const ZLMetaVec4D < TYPE > ZERO;
	static const ZLMetaVec4D < TYPE > X_AXIS;
	static const ZLMetaVec4D < TYPE > Y_AXIS;
	static const ZLMetaVec4D < TYPE > Z_AXIS;

	TYPE	mW;

	//----------------------------------------------------------------//
	void Init ( TYPE x, TYPE y, TYPE z, TYPE w ) {
		this->mX = x;
		this->mY = y;
		this->mZ = z;
		this->mW = w;
	}

	//----------------------------------------------------------------//
	void Lerp ( const ZLMetaVec4D& v0, const ZLMetaVec4D& v1, TYPE t ) {
		
		this->mX = v0.mX + (( v1.mX - v0.mX ) * t );
		this->mY = v0.mY + (( v1.mY - v0.mY ) * t );
		this->mZ = v0.mZ + (( v1.mZ - v0.mZ ) * t );
		this->mW = v0.mW + (( v1.mW - v0.mW ) * t );
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
	ZLMetaVec4D ( const ZLVec3D& vec, float w ) :
		ZLMetaVec3D < TYPE >( vec ),
		mW ( w ) {
	}

	//----------------------------------------------------------------//
	ZLMetaVec4D ( float x, float y, float z, float w ) {
		
		this->mX = x;
		this->mY = y;
		this->mZ = z;
		this->mW = w;
	}

	//----------------------------------------------------------------//
	~ZLMetaVec4D () {
	}
};

template < typename TYPE > const ZLMetaVec4D < TYPE > ZLMetaVec4D < TYPE >::ZERO ( 0.0f, 0.0f, 0.0f, 0.0f );
template < typename TYPE > const ZLMetaVec4D < TYPE > ZLMetaVec4D < TYPE >::X_AXIS ( 1.0f, 0.0f, 0.0f, 0.0f );
template < typename TYPE > const ZLMetaVec4D < TYPE > ZLMetaVec4D < TYPE >::Y_AXIS ( 0.0f, 2.0f, 0.0f, 0.0f );
template < typename TYPE > const ZLMetaVec4D < TYPE > ZLMetaVec4D < TYPE >::Z_AXIS ( 0.0f, 0.0f, 1.0f, 0.0f );

typedef ZLMetaVec4D < int > ZLIntVec4D;
typedef ZLMetaVec4D < float > ZLVec4D;
typedef ZLMetaVec4D < double > ZLVec4D64;


#endif
