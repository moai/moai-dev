// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLRHOMBUS_H
#define	ZLRHOMBUS_H

#include <zl-util/ZLMatrix4x4.h>
#include <zl-util/ZLRect.h>
#include <zl-util/ZLVec3D.h>

class ZLBox;
class ZLPlane3D;

//================================================================//
// ZLRhombus
//================================================================//
class ZLRhombus {
	public:

	ZLVec3D		mLoc;
	ZLVec3D		mXAxis;
	ZLVec3D		mYAxis;

	//----------------------------------------------------------------//
	void	GetAABB		( ZLBox& box ) const;
	void	GetCenter	( ZLVec3D& center ) const;
	void	GetPlane	( ZLPlane3D& plane ) const;
	void	InitXY		( const ZLRect& rect, float zOff );
	void	InitXZ		( const ZLRect& rect, float yOff );
	void	InitZY		( const ZLRect& rect, float xOff );
	void	Transform	( const ZLMatrix4x4& mtx );
};

#endif
