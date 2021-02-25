// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLBOX_H
#define	ZLBOX_H

#include <zl-core/ZLVec3D.h>
#include <zl-core/ZLAffine3D.h>
#include <zl-core/ZLMatrix4x4.h>

class ZLBoxCorners;
class ZLPrism;

//================================================================//
// ZLBox
//================================================================//
class ZLBox {
public:

	static const ZLBox EMPTY;

	enum {
		PLANE_XY,
		PLANE_XZ,
		PLANE_YZ,
	};

	ZLVec3D		mMin;
	ZLVec3D		mMax;

	//----------------------------------------------------------------//
	ZLReal		Area			() const;
	void		Bless			();
	void		Clip			( const ZLBox& box );
	bool		Contains		( const ZLVec3D& loc ) const;
	bool		Contains		( const ZLVec3D& loc, u32 plane ) const;
	void		GetCenter		( ZLVec3D& center ) const;
	void		GetCorners		( ZLVec3D* corners ) const;
	void		GetFitting		( const ZLBox& target, ZLVec3D& offset, ZLVec3D& scale ) const; // gets mapping from self to target
	ZLReal		GetMaxExtent	() const;
	ZLReal		GetRadius		() const;
	ZLRect		GetRect			( u32 plane = PLANE_XY ) const;
	void		Grow			( const ZLBox& box, bool first = false );
	void		Grow			( const ZLRect& rect, bool first = false, u32 plane = PLANE_XY );
	void		Grow			( const ZLVec3D& vec, bool first = false );
	void		Inflate			( ZLReal size );
	void		Init			( const ZLBox& box );
	void		Init			( const ZLBoxCorners& corners );
	void		Init			( const ZLPrism& prism );
	void		Init			( const ZLVec3D& vec );
	void		Init			( const ZLRect& rect, u32 plane = PLANE_XY, ZLReal back = 0.0, ZLReal front = 0.0 );
	void		Init			( ZLReal left, ZLReal top, ZLReal right, ZLReal bottom, ZLReal back, ZLReal front );
	bool		IsPoint			();
	bool		IsSame			( const ZLBox& box ) const;
	void		Offset			( const ZLVec3D& offset );
	bool		Overlap			( const ZLBox& box ) const; // True if boxes overlap
	bool		Overlap			( const ZLBox& box, u32 plane ) const;
	void		Pad				( ZLReal pad ); // Add padding to all sides; so dim += pad * 2 for all axes
	void		Pad				( ZLReal xPad, ZLReal yPad, ZLReal zPad );
	void		Scale			( ZLReal scale );
	void		Scale			( const ZLVec3D& scale );
	void		Transform		( const ZLAffine3D& mtx ); // Gets AABB of transformed box
	void		Transform		( const ZLMatrix4x4& mtx ); // Gets AABB of transformed box
				ZLBox			();
				ZLBox			( const ZLBox& box );
				ZLBox			( const ZLBoxCorners& corners );
				ZLBox			( const ZLPrism& prism );
				ZLBox			( const ZLVec3D& vec );
				ZLBox			( const ZLRect& rect, u32 plane = PLANE_XY, ZLReal back = 0.0, ZLReal front = 0.0 );
				ZLBox			( ZLReal left, ZLReal top, ZLReal right, ZLReal bottom, ZLReal back, ZLReal front );
	
	//----------------------------------------------------------------//
	inline ZLReal Depth () const {
		ZLReal s = this->mMax.mZ - this->mMin.mZ;
		return s < 0.0 ? -s : s;
	}
	
	//----------------------------------------------------------------//
	inline ZLReal Height () const {
		ZLReal s = this->mMax.mY - this->mMin.mY;
		return s < 0.0 ? -s : s;
	}
	
	//----------------------------------------------------------------//
	inline ZLReal Width () const {
		ZLReal s = this->mMax.mX - this->mMin.mX;
		return s < 0.0 ? -s : s;
	}
};

#endif
