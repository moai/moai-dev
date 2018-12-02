// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAW_H
#define	MOAIDRAW_H

#include <moai-sim/MOAIDrawShapeImmediate.h>

class MOAIAnimCurve;
class MOAITextureBase;
class MOAITexture;
class MOAIFont;
class MOAIGlyph;
class MOAIGfxBuffer;
class MOAIVertexFormat;

//================================================================//
// MOAIDraw
//================================================================//
/**	@lua	MOAIDraw
	@text	Singleton for performing immediate mode drawing operations.
			See MOAIDrawDeck.
*/
class MOAIDraw :
	public ZLContextClass < MOAIDraw, MOAILuaObject >,
	public MOAIDrawShapeImmediate {
private:

	//----------------------------------------------------------------//
	static int			_bind					( lua_State* L );
	static int			_bindFrameBuffer		( lua_State* L );
	static int			_bindIndexBuffer		( lua_State* L );
	static int			_bindShader				( lua_State* L );
	static int			_bindTexture			( lua_State* L );
	static int			_bindVertexArray		( lua_State* L );
	static int			_bindVertexBuffer		( lua_State* L );
	static int			_bindVertexFormat		( lua_State* L );
	static int			_clear					( lua_State* L );
	static int			_drawAnimCurve			( lua_State* L );
	static int			_drawAxisGrid			( lua_State* L );
	static int			_drawBezierCurve		( lua_State* L );
	static int			_drawBoxOutline			( lua_State* L );
	static int			_drawCircle				( lua_State* L );
	static int			_drawCircleSpokes		( lua_State* L );
	static int			_drawElements			( lua_State* L );
	static int			_drawEllipse			( lua_State* L );
	static int			_drawEllipseSpokes		( lua_State* L );
	static int			_drawGrid				( lua_State* L );
	static int			_drawLine				( lua_State* L );
	static int			_drawPoints				( lua_State* L );
	static int			_drawRay				( lua_State* L );
	static int			_drawRect				( lua_State* L );
	static int			_drawTexture			( lua_State* L );
	static int			_drawText				( lua_State* L );
	static int			_fillCircle				( lua_State* L );
	static int			_fillEllipse			( lua_State* L );
	static int			_fillFan				( lua_State* L );
	static int			_fillRect				( lua_State* L );
	static int			_fillRoundedRect		( lua_State* L );
	static int			_getPenColor			( lua_State* L );
	static int			_popGfxState			( lua_State* L );
	static int			_pushGfxState			( lua_State* L );
	static int			_setBlendMode			( lua_State* L );
	static int			_setClearColor			( lua_State* L );
	static int			_setClearDepth			( lua_State* L );
	static int			_setCullMode			( lua_State* L );
	static int			_setDefaultTexture		( lua_State* L );
	static int			_setDepthFunc			( lua_State* L );
	static int			_setMatrix				( lua_State* L );
	static int			_setPenColor			( lua_State* L );
	static int			_setPenWidth			( lua_State* L );
	static int			_setScissorRect			( lua_State* L );
	static int			_setViewProj			( lua_State* L );
	static int			_setViewRect			( lua_State* L );
	static int			_strokeRoundedRect		( lua_State* L );
	
	//----------------------------------------------------------------//
	void			DrawLuaParams			( lua_State* L, u32 primType );
	void			DrawLuaArray			( lua_State* L, u32 primType );
	
public:

	DECL_LUA_SINGLETON ( MOAIDraw )

	//----------------------------------------------------------------//
	void			BeginDrawString			( float scale, MOAIFont& font, float fontSize, float shadowOffsetX, float shadowOffsetY );
	bool			Bind					();
	void			DrawAnimCurve			( const MOAIAnimCurve& curve, u32 resolution );
	void			DrawAxisGrid			( ZLVec2D loc, ZLVec2D vec, float size );
	void			DrawBezierCurve			( const ZLCubicBezier2D& bezier );
	void			DrawElements			( MOAIGfxBuffer* vtxBuffer, MOAIVertexFormat* vtxFormat, u32 count );
	void			DrawGrid				( const ZLRect& rect, u32 xCells, u32 yCells );
	void			DrawRay					( float x, float y, float dx, float dy );
	void			DrawString				( cc8* text, float x, float y, float width, float height );
	void			DrawString				( cc8* text, float x, float y, float scale, MOAIFont& font, float fontSize, float shadowOffsetX, float shadowOffsetY, float width, float height );
	void			DrawTexture				( float left, float top, float right, float bottom, MOAITexture* texture );
	void			DrawVertexArray			( const ZLVec3D* verts, u32 count, u32 color, u32 primType );
	void			DrawVertexArray2D		( const float* verts, u32 count, u32 color, u32 primType );
	void			EndDrawString			();
					MOAIDraw				();
					~MOAIDraw				();
	void			RegisterLuaClass		( MOAILuaState& state );
};

#endif
