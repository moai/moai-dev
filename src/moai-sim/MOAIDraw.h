// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAW_H
#define	MOAIDRAW_H

class MOAIAnimCurve;
class MOAITextureBase;
class MOAITexture;
class MOAIFont;
class MOAIGlyph;

//================================================================//
// MOAIDraw
//================================================================//
/**	@lua	MOAIDraw
	@text	Singleton for performing immediate mode drawing operations.
			See MOAIScriptDeck.
*/
class MOAIDraw :
	public MOAIGlobalClass < MOAIDraw, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int				_drawAnimCurve		( lua_State* L );
	static int				_drawAxisGrid		( lua_State* L );
	static int				_drawBezierCurve	( lua_State* L );
	static int				_drawBoxOutline		( lua_State* L );
	static int				_drawCircle			( lua_State* L );
	static int				_drawEllipse		( lua_State* L );
	static int				_drawGrid			( lua_State* L );
	static int				_drawLine			( lua_State* L );
	static int				_drawPoints			( lua_State* L );
	static int				_drawRay			( lua_State* L );
	static int				_drawRect			( lua_State* L );
	static int				_drawTexture		( lua_State* L );
	static int				_drawText			( lua_State* L );
	static int				_fillCircle			( lua_State* L );
	static int				_fillEllipse		( lua_State* L );
	static int				_fillFan			( lua_State* L );
	static int				_fillRect			( lua_State* L );
	static int				_setBlendMode		( lua_State* L );
	
	//----------------------------------------------------------------//
	static void			DrawLuaParams			( lua_State* L, u32 primType );
	static void			DrawLuaArray			( lua_State* L, u32 primType );
	
public:

	DECL_LUA_SINGLETON ( MOAIDraw )

	//----------------------------------------------------------------//
	static void			BeginDrawString			( float scale, MOAIFont& font, float fontSize, float shadowOffsetX, float shadowOffsetY );
	static void			Bind					();
	static void			DrawAnimCurve			( const MOAIAnimCurve& curve, u32 resolution );
	static void			DrawAxisGrid			( ZLVec2D loc, ZLVec2D vec, float size );
	static void			DrawBezierCurve			( const ZLCubicBezier2D& bezier );
	static void			DrawBoxAxis				( const ZLBox& box );
	static void			DrawBoxDiagonals		( const ZLBox& box );
	static void			DrawBoxOutline			( const ZLBox& box );
	static void			DrawEllipseFill			( const ZLRect& rect, u32 steps );
	static void			DrawEllipseFill			( float x, float y, float xRad, float yRad, u32 steps );
	static void			DrawEllipseOutline		( const ZLRect& rect, u32 steps );
	static void			DrawEllipseOutline		( float x, float y, float xRad, float yRad, u32 steps );
	static void			DrawGrid				( const ZLRect& rect, u32 xCells, u32 yCells );
	static void			DrawLine				( const ZLVec2D& v0, const ZLVec2D& v1 );
	static void			DrawLine				( const ZLVec3D& v0, const ZLVec3D& v1 );
	static void			DrawLine				( float x0, float y0, float x1, float y1 );
	static void			DrawLine				( float x0, float y0, float z0, float x1, float y1, float z1 );
	static void			DrawPoint				( const ZLVec2D& loc );
	static void			DrawPoint				( float x, float y );
	static void			DrawQuad				( const ZLQuad& quad );
	static void			DrawRay					( float x, float y, float dx, float dy );
	static void			DrawRectEdges			( ZLRect rect, u32 edges );
	static void			DrawRectFill			( ZLRect rect, bool asTriStrip = true );
	static void			DrawRectFill			( float left, float top, float right, float bottom, bool asTriStrip = true );
	static void			DrawRectOutline			( const ZLRect& rect );
	static void			DrawRectOutline			( float left, float top, float right, float bottom );
	static void			DrawString				( cc8* text, float x, float y, float width, float height );
	static void			DrawString				( cc8* text, float x, float y, float scale, MOAIFont& font, float fontSize, float shadowOffsetX, float shadowOffsetY, float width, float height );
	static void			DrawTexture				( float left, float top, float right, float bottom, MOAITexture* texture );
	static void			DrawVertexArray			( const ZLVec3D* verts, u32 count, u32 color, u32 primType );
	static void			DrawVertexArray2D		( const float* verts, u32 count, u32 color, u32 primType );
	static void			EndDrawString			();
						MOAIDraw				();
						~MOAIDraw				();
	void				RegisterLuaClass		( MOAILuaState& state );
};

#endif
