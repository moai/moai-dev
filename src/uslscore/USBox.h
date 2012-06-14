// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef BOX_H
#define	BOX_H

#include <uslscore/USAffine3D.h>
#include <uslscore/USMatrix4x4.h>
#include <uslscore/USPrism.h>
#include <uslscore/USVec3D.h>

//================================================================//
// USBox
//================================================================//
class USBox {
public:

	enum {
		PLANE_XY,
		PLANE_XZ,
		PLANE_YZ,
	};

	USVec3D		mMin;
	USVec3D		mMax;

	//----------------------------------------------------------------//
	float		Area			() const;
	void		Bless			();
	void		Clip			( const USBox& box );
	bool		Contains		( const USVec3D& loc ) const;
	bool		Contains		( const USVec3D& loc, u32 plane ) const;
	void		GetCenter		( USVec3D& center ) const;
	void		GetFitting		( const USBox& target, USVec3D& offset, USVec3D& scale ) const; // gets mapping from self to target
	float		GetMaxExtent	() const;
	float		GetRadius		() const;
	USRect		GetRect			( u32 plane ) const;
	void		Grow			( const USBox& box );
	void		Grow			( const USVec3D& vec );
	void		Inflate			( float size );
	void		Init			( const USBox& box );
	void		Init			( const USPrism& prism );
	void		Init			( const USVec3D& vec );
	void		Init			( float left, float top, float right, float bottom, float back, float front );
	bool		IsPoint			();
	void		Offset			( const USVec3D& offset );
	bool		Overlap			( const USBox& box ) const; // True if boxes overlap
	bool		Overlap			( const USBox& box, u32 plane ) const;
	void		Pad				( float pad ); // Add padding to all sides; so dim += pad * 2 for all axes
	void		Scale			( float scale );
	void		Scale			( const USVec3D& scale );
	void		Transform		( const USAffine3D& mtx ); // Gets AABB of transformed box
	void		Transform		( const USMatrix4x4& mtx ); // Gets AABB of transformed box
	
	//----------------------------------------------------------------//
	inline float Depth () const {
		float s = this->mMax.mZ - this->mMin.mZ;
		return s < 0.0f ? -s : s;
	}
	
	//----------------------------------------------------------------//
	inline float Height () const {
		float s = this->mMax.mY - this->mMin.mY;
		return s < 0.0f ? -s : s;
	}
	
	//----------------------------------------------------------------//
	inline float Width () const {
		float s = this->mMax.mX - this->mMin.mX;
		return s < 0.0f ? -s : s;
	}
};

#endif
