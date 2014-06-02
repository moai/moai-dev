// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com
#ifndef	MOAIQUADBRUSH_H
#define	MOAIQUADBRUSH_H

#include <moai-sim/MOAIGfxDevice.h>

//================================================================//
// MOAIQuadBrush
//================================================================//
class MOAIQuadBrush {
private:

	ZLVec2D mVtx [ 4 ];
	ZLVec2D mUV [ 4 ];
	
	//----------------------------------------------------------------//
	static inline void WriteVertex ( MOAIGfxDevice& gfxDevice, const ZLVec2D& vtx, const ZLVec2D& uv ) {
		
		gfxDevice.WriteVtx ( vtx );
		gfxDevice.WriteUV ( uv );
		gfxDevice.WriteFinalColor4b ();
	}

public:

	//----------------------------------------------------------------//
	static void			BindVertexFormat	( MOAIGfxDevice& gfxDevice );
	void				Draw				();
	void				Draw				( float xOff, float yOff, float zOff );
	void				Draw				( float xOff, float yOff, float zOff, float xScale, float yScale );
	void				Draw				( float xOff, float yOff, float zOff, float xScale, float yScale, float uOff, float vOff, float uScale, float vScale );
	ZLRect				GetUVBounds			();
	ZLRect				GetVtxBounds		();	
	void				ScaleUVs			( float xScale, float yScale );
	void				ScaleVerts			( float xScale, float yScale );
	void				SetUVs				( const ZLRect& rect );
	void				SetUVs				( float* buffer );
	void				SetUVs				( float x0, float y0, float x1, float y1 );
	void				SetUVs				( const ZLVec2D& v0, const ZLVec2D& v1, const ZLVec2D& v2, const ZLVec2D& v3 );
	void				SetUVs				( float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3 );
	void				SetVerts			( const ZLRect& rect );
	void				SetVerts			( float* buffer );
	void				SetVerts			( float x0, float y0, float x1, float y1 );
	void				SetVerts			( const ZLVec2D& v0, const ZLVec2D& v1, const ZLVec2D& v2, const ZLVec2D& v3 );
	void				SetVerts			( float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3 );
	void				SetVerts			( const ZLVec2D& v0, float radius );
	void				TransformUVs		( const ZLAffine3D& mtx );
	void				TransformVerts		( const ZLAffine3D& mtx );
};

#endif
