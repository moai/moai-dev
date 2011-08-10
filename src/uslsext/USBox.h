// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef BOX_H
#define	BOX_H

#include <uslsext/USPlane.h>
#include <uslsext/USMatrix4x4.h>

//================================================================//
// USBox
//================================================================//
class USBox {
	public:

	USVec3D		mMin;
	USVec3D		mMax;

	//----------------------------------------------------------------//
	void	Bless		();
	void	Clip		( USBox& box );
	bool	Contains	( USVec3D& loc );
	bool	ContainsX	( float x );
	bool	ContainsY	( float y );
	bool	ContainsZ	( float z );
	float	GetRadius	();
	void	GetRectXY	( USRect& rect );
	void	GetRectXZ	( USRect& rect );
	void	GetRectZY	( USRect& rect );
	void	Grow		( USBox& box );
	void	Grow		( USVec3D& vec );
	void	Init		( USBox& box );
	void	Init		( USVec3D& vec );
	bool	Overlap		( USBox& box ); // True if boxes overlap
	bool	OverlapXY	( USBox& box ); // True if boxes overlap in XY plane
	bool	OverlapXZ	( USBox& box ); // True if boxes overlap in XZ plane
	bool	OverlapZY	( USBox& box ); // True if boxes overlap in ZY plane
	void	Pad			( float pad ); // Add padding to all sides; so dim += pad * 2 for all axes
	void	Scale		( float scale );
	void	Transform	( USMatrix4x4& mtx ); // Gets AABB of transformed box
};

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

//================================================================//
// USPrism
//================================================================//
class USPrism {
	public:

	USVec3D		mLoc;
	USVec3D		mXAxis;
	USVec3D		mYAxis;
	USVec3D		mZAxis;

	//----------------------------------------------------------------//
	void	GetAABB		( USBox& box );
	void	GetCenter	( USVec3D& center );
	void	Init		( USBox& box );
	void	Transform	( USMatrix4x4& mtx );
};

#endif
