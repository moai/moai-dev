// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef BOX_H
#define	BOX_H

#include <uslscore/USMatrix4x4.h>
#include <uslscore/USVec3D.h>

//================================================================//
// USBox
//================================================================//
class USBox {
	public:

	USVec3D		mMin;
	USVec3D		mMax;

	//----------------------------------------------------------------//
	float		Area			() const;
	void		Bless			();
	void		Clip			( const USBox& box );
	bool		Contains		( const USVec3D& loc ) const;
	bool		ContainsX		( float x ) const;
	bool		ContainsY		( float y ) const;
	bool		ContainsZ		( float z ) const;
	void		GetCenter		( USVec3D& center ) const;
	float		GetRadius		() const;
	void		GetRectXY		( USRect& rect ) const;
	void		GetRectXZ		( USRect& rect ) const;
	void		GetRectZY		( USRect& rect ) const;
	void		Grow			( const USBox& box );
	void		Grow			( const USVec3D& vec );
	void		Init			( const USBox& box );
	void		Init			( const USVec3D& vec );
	void		Init			( float left, float top, float right, float bottom, float back, float front );
	bool		Overlap			( const USBox& box ) const; // True if boxes overlap
	bool		OverlapXY		( const USBox& box ) const; // True if boxes overlap in XY plane
	bool		OverlapXZ		( const USBox& box ) const; // True if boxes overlap in XZ plane
	bool		OverlapZY		( const USBox& box ) const; // True if boxes overlap in ZY plane
	void		Pad				( float pad ); // Add padding to all sides; so dim += pad * 2 for all axes
	void		Scale			( float scale );
	void		Transform		( const USMatrix4x4& mtx ); // Gets AABB of transformed box
};

#endif
