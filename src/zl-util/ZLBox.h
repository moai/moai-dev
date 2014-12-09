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
	real		Area			() const;
	void		Bless			();
	void		Clip			( const ZLBox& box );
	bool		Contains		( const ZLVec3D& loc ) const;
	bool		Contains		( const ZLVec3D& loc, u32 plane ) const;
	void		GetCenter		( ZLVec3D& center ) const;
	void		GetFitting		( const ZLBox& target, ZLVec3D& offset, ZLVec3D& scale ) const; // gets mapping from self to target
	real		GetMaxExtent	() const;
	real		GetRadius		() const;
	ZLRect		GetRect			( u32 plane ) const;
	void		Grow			( const ZLBox& box );
	void		Grow			( const ZLVec3D& vec );
	void		Inflate			( real size );
	void		Init			( const ZLBox& box );
	void		Init			( const ZLPrism& prism );
	void		Init			( const ZLVec3D& vec );
	void		Init			( const ZLRect& rect, u32 plane, real back, real front );
	void		Init			( real left, real top, real right, real bottom, real back, real front );
	bool		IsPoint			();
	bool		IsSame			( const ZLBox& box ) const;
	void		Offset			( const ZLVec3D& offset );
	bool		Overlap			( const ZLBox& box ) const; // True if boxes overlap
	bool		Overlap			( const ZLBox& box, u32 plane ) const;
	void		Pad				( real pad ); // Add padding to all sides; so dim += pad * 2 for all axes
	void		Pad				( real xPad, real yPad, real zPad );
	void		Scale			( real scale );
	void		Scale			( const ZLVec3D& scale );
	void		Transform		( const ZLAffine3D& mtx ); // Gets AABB of transformed box
	void		Transform		( const ZLMatrix4x4& mtx ); // Gets AABB of transformed box
	
	//----------------------------------------------------------------//
	inline real Depth () const {
		real s = this->mMax.mZ - this->mMin.mZ;
		return s < 0.0f ? -s : s;
	}
	
	//----------------------------------------------------------------//
	inline real Height () const {
		real s = this->mMax.mY - this->mMin.mY;
		return s < 0.0f ? -s : s;
	}
	
	//----------------------------------------------------------------//
	inline real Width () const {
		real s = this->mMax.mX - this->mMin.mX;
		return s < 0.0f ? -s : s;
	}
};

#endif
