// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USRHOMBUS_H
#define	USRHOMBUS_H

#include <uslscore/USMatrix4x4.h>
#include <uslscore/USRect.h>
#include <uslscore/USVec3D.h>

class USBox;
class USPlane3D;

//================================================================//
// USRhombus
//================================================================//
class USRhombus {
	public:

	USVec3D		mLoc;
	USVec3D		mXAxis;
	USVec3D		mYAxis;

	//----------------------------------------------------------------//
	void	GetAABB		( USBox& box );
	void	GetCenter	( USVec3D& center );
	void	GetPlane	( USPlane3D& plane );
	void	InitXY		( USRect& rect, float zOff );
	void	InitXZ		( USRect& rect, float yOff );
	void	InitZY		( USRect& rect, float xOff );
	void	Transform	( USMatrix4x4& mtx );
};

#endif
