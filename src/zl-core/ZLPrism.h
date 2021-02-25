// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLPRISM_H
#define	ZLPRISM_H

#include <zl-core/ZLAffine3D.h>
#include <zl-core/ZLMatrix4x4.h>
#include <zl-core/ZLRect.h>
#include <zl-core/ZLVec3D.h>

class ZLBox;

//================================================================//
// ZLPrism
//================================================================//
class ZLPrism {
public:

	static const ZLPrism EMPTY;

	ZLVec3D		mLoc;
	ZLVec3D		mXAxis;
	ZLVec3D		mYAxis;
	ZLVec3D		mZAxis;

	//----------------------------------------------------------------//
	void	GetAABB		( ZLBox& box ) const;
	void	GetCenter	( ZLVec3D& center ) const;
	void	Init		( const ZLBox& box );
	void	Transform	( const ZLAffine3D& mtx );
	void	Transform	( const ZLMatrix4x4& mtx );
			ZLPrism		();
			ZLPrism		( const ZLBox& box );
};

#endif
