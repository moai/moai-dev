// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLPRISM_H
#define	ZLPRISM_H

#include <zl-util/ZLAffine3D.h>
#include <zl-util/ZLMatrix4x4.h>
#include <zl-util/ZLRect.h>
#include <zl-util/ZLVec3D.h>

class ZLBox;

//================================================================//
// ZLPrism
//================================================================//
class ZLPrism {
	public:

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
};

#endif
