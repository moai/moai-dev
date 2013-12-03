// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAW_H
#define	MOAIDRAW_H

#include <moaicore/MOAILua.h>

class MOAIAnimCurve;
class MOAITextureBase;
class MOAITexture;

//================================================================//
// MOAIDraw
//================================================================//
/**	@name	MOAIDraw
	@text	Singleton for performing immediate mode drawing operations.
			See MOAIScriptDeck.
*/
class MOAIDraw :
	public MOAIGlobalClass < MOAIDraw, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int				_drawAnimCurve		( lua_State* L );
	static int				_drawAntialiasedLineSegment ( lua_State* L );
	static int				_drawAxisGrid		( lua_State* L );
	static int				_drawBeveledCorner	( lua_State* L );
	static int				_drawBeveledLine	( lua_State* L );
	static int				_drawBeveledLineLoop	( lua_State* L );
	static int				_drawBezierCurve	( lua_State* L );
	static int				_drawBoxOutline		( lua_State* L );
	static int				_drawCardinalSpline ( lua_State* L );
	static int				_drawCardinalSplineLoop ( lua_State* L );
	static int				_drawCardinalSplineStrip ( lua_State* L );
	static int				_drawCatmullRomCurve( lua_State* L );
	static int				_drawCircle			( lua_State* L );
	static int				_drawEllipse		( lua_State* L );
	static int				_drawGrid			( lua_State* L );
	static int				_drawJoinedCorner	( lua_State* L );
	static int				_drawJoinedLine		( lua_State* L );
	static int				_drawJoinedLineLoop	( lua_State* L );
	static int				_drawLine			( lua_State* L );
	static int				_drawPoints			( lua_State* L );
	static int				_drawRay			( lua_State* L );
	static int				_drawRect			( lua_State* L );
	static int				_drawRoundBeveledLine ( lua_State* L );
	static int				_drawRoundedRect	( lua_State* L );
	static int				_fillCenteredRectangularGradient (lua_State* L );
	static int				_fillCircle			( lua_State* L );
	static int				_fillCircularGradient ( lua_State* L );
	static int				_fillCircularSlice	( lua_State* L );
	static int				_fillCircularSliceGradient	( lua_State* L );
	static int				_fillEllipse		( lua_State* L );
	static int				_fillEllipticalGradient	( lua_State* L );
	static int				_fillEllipticalSlice( lua_State* L );
	static int				_fillEllipticalSliceGradient ( lua_State* L );
	static int				_fillHorizontalRectangularGradient ( lua_State* L );
	static int				_fillFan			( lua_State* L );
	static int				_fillRect			( lua_State* L );
	static int				_fillRoundedRect	( lua_State* L );
	static int				_fillRoundedRectangularGradient	( lua_State* L );
	static int				_fillTriangularGradient ( lua_State* L );
	static int				_fillVerticalRectangularGradient ( lua_State* L );
	static int				_drawTexture		( lua_State* L );
	static int				_drawText			( lua_State* L );
	
	//----------------------------------------------------------------//
	static void			DrawLuaParams			( lua_State* L, u32 primType );
	static void			DrawLuaArray			( lua_State* L, u32 primType );

public:

	DECL_LUA_SINGLETON ( MOAIDraw )

	//----------------------------------------------------------------//
	static void			Bind					();
	static void			DrawAntiAliasedLineSegment( float x0, float y0, float x1, float y1, float lineWidth, float blurMargin);
	static void			DrawAnimCurve			( MOAIAnimCurve& curve, u32 resolution );
	static void			DrawAxisGrid			( USVec2D loc, USVec2D vec, float size );
	static void			DrawBeveledCorner		( float x0, float y0, float x1, float y1, float x2, float y2, float lineWidth, float blurMargin );
	static void			DrawBeveledLineLoop		( lua_State* L, float lineWidth, float blurMargin );
	static void			DrawBeveledLines		( lua_State* L, float lineWidth, float blurMargin );
	static void			DrawBezierCurve			( float x0, float y0, float x1, float y1, float cx0, float cy0, float cx1, float cy1, u32 steps );
	static void			DrawBoxOutline			( const USBox& box );
	static void			DrawCardinalSpline		( float x0, float y0, float x1, float y1, float cx0, float cy0, float cx1, float cy1, float tension, u32 steps );
	static void			DrawCardinalSplineLoop	( lua_State* L, float tension, u32 steps);
	static void			DrawCardinalSplineStrip	( lua_State* L, float tension, u32 steps);
	static void			DrawCatmullRomCurve		( float x0, float y0, float x1, float y1, float cx0, float cy0, float cx1, float cy1, u32 steps );
	static void			DrawEllipseFill			( const USRect& rect, u32 steps );
	static void			DrawEllipseFill			( float x, float y, float xRad, float yRad, u32 steps );
	static void			DrawEllipseOutline		( const USRect& rect, u32 steps );
	static void			DrawEllipseOutline		( float x, float y, float xRad, float yRad, u32 steps );
	static void			DrawEllipticalGradientFill ( float x, float y, float xRad, float yRad, u32 steps, const USColorVec &centerColor, const USColorVec &edgeColor );
	static void			DrawEllipticalSliceFill		( float x, float y, float xRad, float yRad, float angle, float offset, float blurMargin, u32 steps );
	static void			DrawEllipticalSliceGradientFill ( float x, float y, float xRad, float yRad, float angle, float offset, float blurMargin, u32 steps, const USColorVec &centerColor, const USColorVec &edgeColor );
	static void			DrawGrid				( const USRect& rect, u32 xCells, u32 yCells );
	static void			DrawJoinedCorner		( float x0, float y0, float x1, float y1, float x2, float y2, float lineWidth, float blurMargin );
	static void			DrawJoinedLine			( lua_State* L, float lineWidth, float blurMargin );
	static void			DrawJoinedLineLoop		( lua_State* L, float lineWidth, float blurMargin );
	static void			DrawLine				( const USVec2D& v0, const USVec2D& v1 );
	static void			DrawLine				( const USVec3D& v0, const USVec3D& v1 );
	static void			DrawLine				( float x0, float y0, float x1, float y1 );
	static void			DrawLine				( float x0, float y0, float z0, float x1, float y1, float z1 );
	static void			DrawPoint				( const USVec2D& loc );
	static void			DrawPoint				( float x, float y );
	static void			DrawQuad				( const USQuad& quad );
	static void			DrawRay					( float x, float y, float dx, float dy );
	static void			DrawRectCenteredGradientFill ( float left, float top, float right, float bottom, const USColorVec &centerColor, const USColorVec &edgeColor, float innerWidth, float innerHeight );
	static void			DrawRectEdges			( USRect rect, u32 edges );
	static void			DrawRectFill			( USRect rect, bool asTriStrip = true );
	static void			DrawRectFill			( float left, float top, float right, float bottom, bool asTriStrip = true );
	static void			DrawRectHorizontalGradientFill	( float left, float top, float right, float bottom, const USColorVec &leftColor, const USColorVec &rightColor );
	static void			DrawRectOutline			( const USRect& rect );
	static void			DrawRectOutline			( float left, float top, float right, float bottom );
	static void			DrawRectVerticalGradientFill ( float left, float top, float right, float bottom, const USColorVec &topColor, const USColorVec &bottomColor );
	static void			DrawRoundBeveledLine	( lua_State* L, float lineWidth, float blurMargin, u32 steps );
	static void			DrawRoundedRectFill		( float left, float top, float right, float bottom, float cornerRadius, float blurMargin, u32 steps );
	static void			DrawRoundedRectGradientFill		( float left, float top, float right, float bottom, float cornerRadius, float blurMargin, u32 steps, const USColorVec &centerColor, const USColorVec &edgeColor );
	static void			DrawRoundedRectOutline	( float left, float top, float right, float bottom, float cornerRadius, u32 steps );
	static void			DrawTexture				( float left, float top, float right, float bottom, MOAITexture* texture );
	static void			DrawTriangularGradientFill (const USVec2D& v0, const USVec2D& v1, const USVec2D& v2, const USColorVec &color0, const USColorVec &color1, const USColorVec &color2);
	static void			DrawVertexArray			( const USVec3D* verts, u32 count, u32 color, u32 primType );
	static void			DrawVertexArray2D		( const float* verts, u32 count, u32 color, u32 primType );
						MOAIDraw				();
						~MOAIDraw				();
	void				RegisterLuaClass		( MOAILuaState& state );
};

#endif
