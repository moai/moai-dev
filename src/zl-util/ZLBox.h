// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLBOX_H
#define	ZLBOX_H

#include <zl-util/ZLAffine3D.h>
#include <zl-util/ZLMatrix4x4.h>
#include <zl-util/ZLPrism.h>
#include <zl-util/ZLVec3D.h>

//================================================================//
// ZLBox
//================================================================//
class ZLBox {
public:

	enum {
		PLANE_XY,
		PLANE_XZ,
		PLANE_YZ,
	};

	ZLVec3D		mMin;
	ZLVec3D		mMax;

	//----------------------------------------------------------------//
	float		Area			() const;
	void		Bless			();
	void		Clip			( const ZLBox& box );
	bool		Contains		( const ZLVec3D& loc ) const;
	bool		Contains		( const ZLVec3D& loc, u32 plane ) const;
	void		GetCenter		( ZLVec3D& center ) const;
	void		GetFitting		( const ZLBox& target, ZLVec3D& offset, ZLVec3D& scale ) const; // gets mapping from self to target
	float		GetMaxExtent	() const;
	float		GetRadius		() const;
	ZLRect		GetRect			( u32 plane ) const;
	void		Grow			( const ZLBox& box );
	void		Grow			( const ZLVec3D& vec );
	void		Inflate			( float size );
	void		Init			( const ZLBox& box );
	void		Init			( const ZLPrism& prism );
	void		Init			( const ZLVec3D& vec );
	void		Init			( const ZLRect& rect, u32 plane = PLANE_XY, float back = 0.0f, float front = 0.0f );
	void		Init			( float left, float top, float right, float bottom, float back, float front );
	bool		IsPoint			();
	bool		IsSame			( const ZLBox& box ) const;
	void		Offset			( const ZLVec3D& offset );
	bool		Overlap			( const ZLBox& box ) const; // True if boxes overlap
	bool		Overlap			( const ZLBox& box, u32 plane ) const;
	void		Pad				( float pad ); // Add padding to all sides; so dim += pad * 2 for all axes
	void		Pad				( float xPad, float yPad, float zPad );
	void		Scale			( float scale );
	void		Scale			( const ZLVec3D& scale );
	void		Transform		( const ZLAffine3D& mtx ); // Gets AABB of transformed box
	void		Transform		( const ZLMatrix4x4& mtx ); // Gets AABB of transformed box
	
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
