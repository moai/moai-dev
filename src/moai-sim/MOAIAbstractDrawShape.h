// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTDRAWSHAPE_H
#define	MOAIABSTRACTDRAWSHAPE_H

//================================================================//
// MOAIAbstractDrawShape
//================================================================//
// TODO: doxygen
class MOAIAbstractDrawShape {
private:

	//----------------------------------------------------------------//
	// TODO: not ready for prime time; see notes in implementation
	void			DrawArcFill					( float x, float y, float xRad, float yRad, float d0, float d1, u32 steps );
	void			DrawArcStroke				( float x, float y, float xRad, float yRad, float d0, float d1, u32 steps, float stroke, float offset );

protected:

	//----------------------------------------------------------------//
	virtual void		MOAIAbstractDrawShape_DrawLine				( float x0, float y0, float z0, float x1, float y1, float z1 ) = 0;
	virtual void		MOAIAbstractDrawShape_DrawPoint				( float x, float y, float z = 0.0f ) = 0;
	virtual void		MOAIAbstractDrawShape_DrawTriangleFill		( const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2 ) = 0;
	virtual void		MOAIAbstractDrawShape_SetPenColor			( u32 color ) = 0;
	virtual void		MOAIAbstractDrawShape_SetPenWidth			( float width ) = 0;

public:

	//----------------------------------------------------------------//
	void			DrawBoxAxis					( const ZLBox& box );
	void			DrawBoxDiagonals			( const ZLBox& box );
	void			DrawBoxOutline				( const ZLBox& box );
	void			DrawCircleFill				( float x, float y, float radius, u32 steps );
	void			DrawCircleOutline			( float x, float y, float radius, u32 steps );
	void			DrawEllipseFill				( const ZLRect& rect, u32 steps );
	void			DrawEllipseFill				( float x, float y, float xRad, float yRad, u32 steps );
	void			DrawEllipseOutline			( const ZLRect& rect, u32 steps );
	void			DrawEllipseOutline			( float x, float y, float xRad, float yRad, u32 steps );
	void			DrawEllipseSpokes			( float x, float y, float xRad, float yRad, u32 steps );
	void			DrawLine					( const ZLVec2D& v0, const ZLVec2D& v1 );
	void			DrawLine					( const ZLVec3D& v0, const ZLVec3D& v1 );
	void			DrawLine					( float x0, float y0, float x1, float y1 );
	void			DrawLine					( float x0, float y0, float z0, float x1, float y1, float z1 );
	void			DrawPoint					( const ZLVec2D& loc );
	void			DrawPoint					( float x, float y, float z = 0.0f );
	void			DrawPolyOutline				( const ZLPolygon2D& poly );
	void			DrawQuadOutline				( const ZLQuad& quad );
	void			DrawRay						( float x, float y, float dx, float dy, float length );
	void			DrawRectEdges				( ZLRect rect, u32 edges );
	void			DrawRectFill				( ZLRect rect );
	void			DrawRectFill				( float left, float top, float right, float bottom );
	void			DrawRectOutline				( const ZLRect& rect );
	void			DrawRectOutline				( float left, float top, float right, float bottom );
	void			DrawRoundedRectFill			( float left, float top, float right, float bottom, float xRad, float yRad, u32 steps );
	void			DrawRoundedRectStroke		( float left, float top, float right, float bottom, float xRad, float yRad, u32 steps, float stroke, float offset );
	void			DrawTriangleFill			( const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2 );
	void			DrawTriangleOutline			( const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2 );
					MOAIAbstractDrawShape		();
	virtual			~MOAIAbstractDrawShape		();
	void			SetPenColor					( u32 color );
	void			SetPenWidth					( float width );
};

#endif
