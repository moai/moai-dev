// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com
#ifndef	MOAIQUADBRUSH_H
#define	MOAIQUADBRUSH_H

#include <moaicore/MOAIGfxDevice.h>

//================================================================//
// MOAIQuadBrush
//================================================================//
class MOAIQuadBrush {
private:

	USVec2D mVtx [ 4 ];
	USVec2D mUV [ 4 ];

	//----------------------------------------------------------------//
	void				Draw				( const USVec2D* vtx, const USVec2D* uv );
	
	//----------------------------------------------------------------//
	static inline void WriteVertex ( MOAIGfxDevice& gfxDevice, const USVec2D& vtx, const USVec2D& uv ) {
		
		gfxDevice.WriteVtx ( vtx );
		gfxDevice.WriteUV ( uv );
		gfxDevice.WriteFinalColor4b ();
	}

public:

	//----------------------------------------------------------------//
	static void			BindVertexFormat	( MOAIGfxDevice& gfxDevice );
	void				Draw				();
	void				Draw				( const USMatrix3x3& mtx );
	void				Draw				( float xOff, float yOff );
	void				Draw				( float xOff, float yOff, float xScale, float yScale );
	USRect				GetUVBounds			();
	USRect				GetVtxBounds		();	
	void				ScaleUVs			( float xScale, float yScale );
	void				ScaleVerts			( float xScale, float yScale );
	void				SetUVs				( const USRect& rect );
	void				SetUVs				( float* buffer );
	void				SetUVs				( float x0, float y0, float x1, float y1 );
	void				SetUVs				( const USVec2D& v0, const USVec2D& v1, const USVec2D& v2, const USVec2D& v3 );
	void				SetUVs				( float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3 );
	void				SetVerts			( const USRect& rect );
	void				SetVerts			( float* buffer );
	void				SetVerts			( float x0, float y0, float x1, float y1 );
	void				SetVerts			( const USVec2D& v0, const USVec2D& v1, const USVec2D& v2, const USVec2D& v3 );
	void				SetVerts			( float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3 );
	void				SetVerts			( const USVec2D& v0, float radius );
	void				TransformUVs		( const USMatrix3x3& mtx );
	void				TransformVerts		( const USMatrix3x3& mtx );
};

#endif
