// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAW_H
#define	MOAIDRAW_H

class USTexture;

//================================================================//
// MOAIDraw
//================================================================//
class MOAIDraw {
public:

	//----------------------------------------------------------------//
	static void			DrawAxisGrid			( USVec2D loc, USVec2D vec, float size );
	static void			DrawEllipseFill			( USRect& rect, u32 steps );
	static void			DrawEllipseFill			( USVec2D& loc, float rad, u32 steps );
	static void			DrawEllipseFill			( USVec2D& loc, float xRad, float yRad, u32 steps );
	static void			DrawEllipseFill			( float left, float top, float right, float bottom , u32 steps);
	static void			DrawEllipseOutline		( USRect& rect, u32 steps );
	static void			DrawEllipseOutline		( USVec2D& loc, float rad, u32 steps );
	static void			DrawEllipseOutline		( USVec2D& loc, float xRad, float yRad, u32 steps );
	static void			DrawEllipseOutline		( float left, float top, float right, float bottom, u32 steps );
	static void			DrawGrid				( USRect& rect, u32 xCells, u32 yCells );
	static void			DrawLine				( USVec2D& v0, USVec2D& v1 );
	static void			DrawLine				( float x0, float y0, float x1, float y1 );
	static void			DrawPoint				( USVec2D& loc );
	static void			DrawPoint				( float x, float y );
	static void			DrawRay					( USVec2D loc, USVec2D vec );
	static void			DrawRectEdges			( USRect rect, u32 edges );
	static void			DrawRectFill			( USRect rect );
	static void			DrawRectFill			( float left, float top, float right, float bottom );
	static void			DrawRectOutline			( USRect& rect );
	static void			DrawRectOutline			( float left, float top, float right, float bottom );
};

#endif
