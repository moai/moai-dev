// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAnimCurve.h>
#include <moai-sim/MOAIDraw.h>
#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAIGfxBuffer.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAIVertexFormatMgr.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAITexture.h>
#include <moai-sim/MOAITransformBase.h>
#include <moai-sim/MOAIVertexBuffer.h>
#include <moai-sim/MOAIVertexFormat.h>
#include <moai-sim/MOAIVertexFormatMgr.h>
#include <moai-sim/MOAIViewport.h>
#include <moai-sim/MOAIQuadBrush.h>

#define DEFAULT_ELLIPSE_STEPS 64

//================================================================//
// MOAIDrawVertexLineStripWriter2D
//================================================================//
class MOAIDrawVertexLineStripWriter2D :
	public ZLAbstractVertexWriter2D {
private:

	u32				mVertexCount;
	ZLVec2D			mLastVertex;
	MOAIGfxMgr&		mGfxMgr;

public:

	//----------------------------------------------------------------//
	void Begin () {
	
		this->mVertexCount = 0;
		this->mGfxMgr.mVertexCache.BeginPrim ( ZGL_PRIM_LINE_STRIP, 1 );
	}
	
	//----------------------------------------------------------------//
	MOAIDrawVertexLineStripWriter2D () :
		mGfxMgr ( MOAIGfxMgr::Get ()) {
	}

	//----------------------------------------------------------------//
	void WriteVertex ( const ZLVec2D& v ) {
		
		if (( this->mVertexCount > 0 ) && ( this->mGfxMgr.mVertexCache.ContinuePrim ( 1 ) == MOAIGfxVertexCache::CONTINUE_ROLLOVER )) {

			// if we roll over, repeat the last vertex to start a new line strip
			this->mGfxMgr.mVertexCache.WriteVtx ( this->mLastVertex.mX, this->mLastVertex.mY );
			this->mGfxMgr.mVertexCache.WritePenColor4b ();
		}
		
		this->mGfxMgr.mVertexCache.WriteVtx ( v.mX, v.mY );
		this->mGfxMgr.mVertexCache.WritePenColor4b ();
		
		this->mLastVertex = v;
	}
	
	//----------------------------------------------------------------//
	void End () {
	
		this->mGfxMgr.mVertexCache.EndPrim ();
	}
};

//================================================================//
// text drawing stuff
//================================================================//
/*
	TODO: I want to refactor/rewrite this stuff to consolidate the glyph layout and buffering
	under a single code path - there's some redundancy here with MOAITextLabel that can probably
	be smoothed away.
*/

struct GlyphPlacement {

	MOAIGlyph* glyph;
	float x;
	float y;
};

struct TextDrawContext {
		
	// Text
	STLList < GlyphPlacement > mGlyphs;

	// Text data
	MOAIFont* mFont;
	float mScale;
	float mFontSize;
	float mShadowOffsetX;
	float mShadowOffsetY;
};

static TextDrawContext g_TextDrawContext;
static TextDrawContext* g_CurrentTextDrawContext = 0; // TODO: ugh! refactor this!

//----------------------------------------------------------------//
void MOAIDraw::BeginDrawString ( float scale, MOAIFont& font, float fontSize, float shadowOffsetX, float shadowOffsetY ) {
	
	assert ( g_CurrentTextDrawContext == 0 );
	g_CurrentTextDrawContext = &g_TextDrawContext;

	g_CurrentTextDrawContext->mFont = &font;
	g_CurrentTextDrawContext->mFontSize = fontSize;
	g_CurrentTextDrawContext->mScale = scale;
	g_CurrentTextDrawContext->mShadowOffsetX = shadowOffsetX;
	g_CurrentTextDrawContext->mShadowOffsetY = shadowOffsetY;
}

//----------------------------------------------------------------//
void MOAIDraw::DrawString ( cc8* text, float x, float y, float width, float height ) {

	// Sanity check
	size_t textLength = strlen ( text );
	if ( textLength == 0 ) return;
	
	// Get the context data
	assert ( g_CurrentTextDrawContext );

	// Transform the center into 'world' space
	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	const ZLMatrix4x4& orgWorldTransform = gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::MODEL_TO_WORLD_MTX );
	ZLVec2D pos ( x, y );
	orgWorldTransform.Transform ( pos );
	x = pos.mX;
	y = pos.mY;

	// Extract the 'state'
	MOAIFont& font = *g_CurrentTextDrawContext->mFont;
	float scale = g_CurrentTextDrawContext->mScale;
	float fontSize = g_CurrentTextDrawContext->mFontSize;
	
	MOAIGlyphSet* glyphSet = font.GetGlyphSet ( fontSize );
	assert ( glyphSet );

	// Let's draw the string!
	float cursorX = x;
	float cursorY = y + glyphSet->GetAscent() * scale;
	MOAIGlyph* prevGlyph = 0;
	
	// Update the glyph cache
	for ( size_t i = 0; i < textLength; i++ ) {

		cc8 c = text [ i ];
		if ( c != '\n' ) {

			font.AffirmGlyph ( fontSize, c );
		}
	}
	font.ProcessGlyphs ();

	glyphSet = font.GetGlyphSet ( fontSize );
	assert ( glyphSet );

	for ( size_t i = 0; i < textLength; i++ ) {

		cc8 c = text [ i ];
		if ( c == '\n' ) {

			// Move to the next line
			cursorX = x;
			cursorY += glyphSet->GetHeight () * scale;
			prevGlyph = 0;

			if ( height > 0 && (cursorY - y) > height ) {
				break;
			}
		}
		else {

			if ( width > 0 && (cursorX - x) > width ) {
				continue;
			}

			// Get the glyph for the current character
			MOAIGlyph* glyph = glyphSet->GetGlyph ( c );
			if ( glyph ) {

				// Draw the current glyph
				MOAITextureBase* glyphTexture = font.GetGlyphTexture ( *glyph );
				if ( glyphTexture ) {

					GlyphPlacement placement = { glyph, cursorX, cursorY };
					g_CurrentTextDrawContext->mGlyphs.push_back( placement );
				}

				// Apply kerning
				if ( prevGlyph ) {

					MOAIKernVec kern = prevGlyph->GetKerning ( glyph->GetCode () );
					cursorX += kern.mX * scale;
				}

				// Move the cursor
				cursorX += glyph->GetAdvanceX () * scale;
			}

			prevGlyph = glyph;
		}
	}
}

//----------------------------------------------------------------//
void MOAIDraw::DrawString	( cc8* text, float x, float y, float scale, MOAIFont& font, float fontSize, float shadowOffsetX, float shadowOffsetY, float width, float height ) {

	BeginDrawString ( scale, font, fontSize, shadowOffsetX, shadowOffsetY );
	DrawString ( text, x, y, width, height );
	EndDrawString ();
}

//----------------------------------------------------------------//
void MOAIDraw::EndDrawString () {

	// Setup for drawing
	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	// Get current state
	//const ZLMatrix4x4& orgWorldTransform = gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::MODEL_TO_WORLD_MTX );

	// TODO
	//GLint orgSrcBlend, orgDestBlend;
	//glGetIntegerv ( GL_BLEND_SRC, &orgSrcBlend );
	//glGetIntegerv ( GL_BLEND_DST, &orgDestBlend );

	// Apply render state
	if ( !gfxMgr.mGfxState.SetShader ( MOAIShaderMgr::FONT_SHADER )) return;
	
	gfxMgr.mGfxState.SetBlendMode ( ZGL_BLEND_FACTOR_ONE, ZGL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA );
	gfxMgr.mVertexCache.SetVertexTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::MODEL_TO_WORLD_MTX ));
	MOAIQuadBrush::BindVertexFormat ();

	// Get the context data
	assert( g_CurrentTextDrawContext );
	
	//MOAIFont& font = *g_CurrentTextDrawContext->mFont;
	//float scale = g_CurrentTextDrawContext->mScale;
	float shadowOffsetX = g_CurrentTextDrawContext->mShadowOffsetX;
	float shadowOffsetY = g_CurrentTextDrawContext->mShadowOffsetY;

	bool drawDropShadows = fabsf ( shadowOffsetX ) > 0.0001 && fabsf ( shadowOffsetY ) > 0.0001;

	u32 numPasses = 1;
	//float offsetX = 0;
	//float offsetY = 0;
	ZLColorVec penColor = gfxMgr.mGfxState.GetPenColor ();
	if ( drawDropShadows ) {

		numPasses = 2;		
		gfxMgr.mGfxState.SetPenColor ( 0, 0, 0, 1 );
		//offsetX = shadowOffsetX;
		//offsetY = shadowOffsetY;
	}

	for ( u32 pass = 0; pass < numPasses; pass++ ) {

		if ( pass == 1 || numPasses == 1 ) {
			gfxMgr.mGfxState.SetPenColor ( penColor );
			//offsetX = 0;
			//offsetY = 0;
		}

		//STLList < GlyphPlacement >::const_iterator it;
		//for ( it = g_CurrentTextDrawContext->mGlyphs.begin (); it != g_CurrentTextDrawContext->mGlyphs.end (); ++it ) {

		//	const GlyphPlacement& glyphPlacement = *it;
		//	MOAIGlyph* glyph = glyphPlacement.glyph;
		//	MOAITextureBase* glyphTexture = font.GetGlyphTexture ( *glyph );
		//	glyph->Draw ( *glyphTexture, glyphPlacement.x + offsetX, glyphPlacement.y + offsetY, scale, scale );
		//}
	}

	// Restore render state
	MOAIDraw::Bind ();

	//gfxMgr.SetVertexTransform ( MOAIGfxMgr::VTX_WORLD_TRANSFORM, orgWorldTransform );
	//gfxMgr.SetBlendMode ( orgSrcBlend, orgDestBlend ); // TODO

	// Clear context
	g_CurrentTextDrawContext->mFont = 0;
	g_CurrentTextDrawContext->mGlyphs.clear();
	g_CurrentTextDrawContext = 0;
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_bindFrameBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	MOAIGfxMgr::Get ().mGfxState.SetFrameBuffer ( state.GetLuaObject < MOAIFrameBuffer >( 1, false ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_bindIndexBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	MOAIGfxMgr::Get ().mGfxState.SetIndexBuffer ( state.GetLuaObject < MOAIIndexBuffer >( 1, false ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_bindShader ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	switch ( lua_type ( state, 1 )) {
	
		case LUA_TUSERDATA:
			MOAIGfxMgr::Get ().mGfxState.SetShader ( state.GetLuaObject < MOAIShader >( 1, true ));
			break;
		
		case LUA_TNUMBER:
			MOAIGfxMgr::Get ().mGfxState.SetShader (( MOAIShaderMgr::Preset )state.GetValue < u32 >( 1, MOAIShaderMgr::LINE_SHADER ));
			break;
		
		default:
			MOAIGfxMgr::Get ().mGfxState.SetShader ();
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_bindTexture ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	MOAIGfxMgr::Get ().mGfxState.SetTexture ( state.GetLuaObject < MOAITexture >( 1, false ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_bindVertexArray ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	MOAIGfxMgr::Get ().mGfxState.SetVertexArray ( state.GetLuaObject < MOAIVertexArray >( 1, false ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_bindVertexBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	MOAIGfxMgr::Get ().mGfxState.SetVertexBuffer ( state.GetLuaObject < MOAIVertexBuffer >( 1, false ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_bindVertexFormat ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	switch ( lua_type ( state, 1 )) {
	
		case LUA_TUSERDATA:
			MOAIGfxMgr::Get ().mGfxState.SetVertexFormat ( state.GetLuaObject < MOAIVertexFormat >( 1, true ));
			break;
		
		case LUA_TNUMBER:
			MOAIGfxMgr::Get ().mGfxState.SetVertexFormat (( MOAIVertexFormatMgr::Preset )state.GetValue < u32 >( 1, MOAIVertexFormatMgr::XYZWC ));
			break;
		
		default:
			MOAIGfxMgr::Get ().mGfxState.SetVertexFormat ();
			break;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_clear ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	u32 clearFlags = gfxMgr.mGfxState.GetClearFlags ();

	if ( clearFlags & ZGL_CLEAR_COLOR_BUFFER_BIT ) {
	
		const ZLColorVec& clearColor = gfxMgr.mGfxState.GetClearColor ();
		
		MOAIGfxMgr::GetDrawingAPI ().ClearColor (
			clearColor.mR,
			clearColor.mG,
			clearColor.mB,
			clearColor.mA
		);
	}
	
	// TODO
	//if ( clearFlags & ZGL_CLEAR_DEPTH_BUFFER_BIT ) {
	//}

	gfxMgr.ClearSurface ( clearFlags );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_drawAnimCurve ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )

	MOAIAnimCurve* curve	= state.GetLuaObject < MOAIAnimCurve >( 1, true );
	u32 resolution			= state.GetValue < u32 >( 2, 1 );

	if ( curve ) {
		self->DrawAnimCurve ( *curve, resolution );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: implement
int MOAIDraw::_drawAxisGrid ( lua_State* L ) {
	UNUSED ( L );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	drawBezierCurve
	@text	Draws a bezier curve.
	
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@in		number x2
	@in		number y2
	@in		number x3
	@in		number y3
	@out	nil
*/
int MOAIDraw::_drawBezierCurve ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	ZLCubicBezier2D bezier;
	
	bezier.mP0.mX = state.GetValue < float >( 1, 0.0f );
	bezier.mP0.mY = state.GetValue < float >( 2, 0.0f );
	
	bezier.mP1.mX = state.GetValue < float >( 3, 0.0f );
	bezier.mP1.mY = state.GetValue < float >( 4, 0.0f );
	
	bezier.mP2.mX = state.GetValue < float >( 5, 0.0f );
	bezier.mP2.mY = state.GetValue < float >( 6, 0.0f );
	
	bezier.mP3.mX = state.GetValue < float >( 7, 0.0f );
	bezier.mP3.mY = state.GetValue < float >( 8, 0.0f );
	
	self->DrawBezierCurve ( bezier );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	drawBoxOutline
	@text	Draw a box outline.
	
	@in		number x0
	@in		number y0
	@in		number z0
	@in		number x1
	@in		number y1
	@in		number z1
	@out	nil
*/
int MOAIDraw::_drawBoxOutline ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )

	ZLBox box;
	box.mMin.mX = state.GetValue < float >( 1, 0.0f );
	box.mMin.mY = state.GetValue < float >( 2, 0.0f );
	box.mMin.mZ = state.GetValue < float >( 3, 0.0f );
	box.mMax.mX = state.GetValue < float >( 4, box.mMin.mX );
	box.mMax.mY = state.GetValue < float >( 5, box.mMin.mY );
	box.mMax.mZ = state.GetValue < float >( 6, box.mMin.mZ );
	
	self->Get ().DrawBoxOutline ( box );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	drawCircle
	@text	Draw a circle.
	
	@in		number x
	@in		number y
	@in		number r
	@in		number steps
	@out	nil
*/
int MOAIDraw::_drawCircle ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	float x0	= state.GetValue < float >( 1, 0.0f );
	float y0	= state.GetValue < float >( 2, 0.0f );
	float r		= state.GetValue < float >( 3, 0.0f );
	u32 steps	= state.GetValue < u32 >( 4, DEFAULT_ELLIPSE_STEPS );

	self->Get ().DrawEllipseOutline ( x0, y0, r, r, steps );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	drawCircleSpokes
	@text	Draw the spokes of a circle.
	
	@in		number x
	@in		number y
	@in		number r
	@in		number steps
	@out	nil
*/
int MOAIDraw::_drawCircleSpokes ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	float x0	= state.GetValue < float >( 1, 0.0f );
	float y0	= state.GetValue < float >( 2, 0.0f );
	float r		= state.GetValue < float >( 3, 0.0f );
	u32 steps	= state.GetValue < u32 >( 4, DEFAULT_ELLIPSE_STEPS );

	self->Get ().DrawEllipseSpokes ( x0, y0, r, r, steps );
	return 0;
}


//----------------------------------------------------------------//
/**	@lua	drawElements
	@text	Draw elements.
	
	@in		...     vtxBuffer	VertexBuffer
	@in		...     format		Vertex Format of vertices in the buffer
	@in		number  count       Number of indices to be rendered
	@out	nil
 */
int MOAIDraw::_drawElements ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	MOAIGfxBuffer* vtxBuffer = state.GetLuaObject < MOAIGfxBuffer >( 1, false );
	MOAIVertexFormat* format = state.GetLuaObject < MOAIVertexFormat >( 2, false );
	u32 count = state.GetValue < u32 >( 3, 0 );
	
	self->DrawElements ( vtxBuffer, format, count );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	drawEllipse
	@text	Draw an ellipse.
	
	@in		number x
	@in		number y
	@in		number xRad
	@in		number yRad
	@in		number steps
	@out	nil
*/
int MOAIDraw::_drawEllipse ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	float x		= state.GetValue < float >( 1, 0.0f );
	float y		= state.GetValue < float >( 2, 0.0f );
	float xRad	= state.GetValue < float >( 3, 0.0f );
	float yRad	= state.GetValue < float >( 4, 0.0f );
	
	u32 steps = state.GetValue < u32 >( 5, DEFAULT_ELLIPSE_STEPS );

	self->Get ().DrawEllipseOutline ( x, y, xRad, yRad, steps );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	drawCircleSpokes
	@text	Draw the spokes of an ellipse.
	
	@in		number x
	@in		number y
	@in		number xRad
	@in		number yRad
	@in		number steps
	@out	nil
*/
int MOAIDraw::_drawEllipseSpokes ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	float x		= state.GetValue < float >( 1, 0.0f );
	float y		= state.GetValue < float >( 2, 0.0f );
	float xRad	= state.GetValue < float >( 3, 0.0f );
	float yRad	= state.GetValue < float >( 4, 0.0f );

	u32 steps = state.GetValue < u32 >( 5, DEFAULT_ELLIPSE_STEPS );

	self->Get ().DrawEllipseSpokes ( x, y, xRad, yRad, steps );
	return 0;
}

//----------------------------------------------------------------//
// TODO: implement
int MOAIDraw::_drawGrid ( lua_State* L ) {
	UNUSED ( L );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	drawLine
	@text	Draw a line.
	
	@in		... vertices		List of vertices (x, y) or an array of vertices
								{ x0, y0, x1, y1, ... , xn, yn }
	@out	nil
*/
int MOAIDraw::_drawLine ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )

	if ( lua_istable ( L, -1 ) ) {
		self->DrawLuaArray ( L, ZGL_PRIM_LINE_STRIP );
	}
	else {
		self->DrawLuaParams ( L, ZGL_PRIM_LINE_STRIP );
	}
	
	MOAIGfxMgr::Get ().mVertexCache.FlushBufferedPrims (); // TODO: have to do this here?
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	drawPoints
	@text	Draw a list of points.
	
	@in		... vertices		List of vertices (x, y) or an array of vertices
								{ x0, y0, x1, y1, ... , xn, yn }
	@out	nil
*/
int MOAIDraw::_drawPoints ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	if ( lua_istable ( L, -1 ) ) {
		self->DrawLuaArray ( L, ZGL_PRIM_POINTS );
	}
	else {
		self->DrawLuaParams ( L, ZGL_PRIM_POINTS );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	drawRay
	@text	Draw a ray.
	
	@in		number x
	@in		number y
	@in		number dx
	@in		number dy
	@out	nil
*/
int MOAIDraw::_drawRay ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	float x		= state.GetValue < float >( 1, 0.0f );
	float y		= state.GetValue < float >( 2, 0.0f );
	float dx	= state.GetValue < float >( 3, 0.0f );
	float dy	= state.GetValue < float >( 4, 0.0f );

	self->DrawRay ( x, y, dx, dy );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	drawRect
	@text	Draw a rectangle.
	
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@out	nil
*/
int MOAIDraw::_drawRect ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	float x0 = state.GetValue < float >( 1, 0.0f );
	float y0 = state.GetValue < float >( 2, 0.0f );
	float x1 = state.GetValue < float >( 3, 0.0f );
	float y1 = state.GetValue < float >( 4, 0.0f );

	self->Get ().DrawRectOutline ( x0, y0, x1, y1 );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	drawTexture
	@text	Draw a filled rectangle.
	
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@in		MOAITexture texture
	@out	nil
*/
int MOAIDraw::_drawTexture ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	float x0 = state.GetValue < float >( 1, 0.0f );
	float y0 = state.GetValue < float >( 2, 0.0f );
	float x1 = state.GetValue < float >( 3, 0.0f );
	float y1 = state.GetValue < float >( 4, 0.0f );
	MOAITexture* texture = (MOAITexture*)MOAITexture::AffirmTexture ( state, 5 );

	self->DrawTexture ( x0, y0, x1, y1, texture );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	drawText
	@text	Draws a string.
	
	@in		MOAIFont font
	@in		number size		Font size
	@in		string text
	@in		number x		Left position
	@in		number y		Top position
	@in		number scale
	@in		number shadowOffsetX
	@in		number shadowOffsetY
	@out	nil
*/
int MOAIDraw::_drawText ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )

	// TODO	
	//cc8* text = lua_tostring ( state, 3 );
	//if ( text ) {

	//	float x = state.GetValue < float >( 4, 0.0f );
	//	float y = state.GetValue < float >( 5, 0.0f );
	//	float scale = state.GetValue < float >( 6, 1.0f );

	//	float shadowOffsetX = state.GetValue < float >( 7, 0.0f );
	//	float shadowOffsetY = state.GetValue < float >( 8, 0.0f );

	//	MOAIFont* font = state.GetLuaObject < MOAIFont >( 1, true );
	//	if ( font ) {

	//		float fontSize = state.GetValue < float >( 2, font->GetDefaultSize () );

	//		MOAIDraw::DrawText ( text, x, y, scale, *font, fontSize, shadowOffsetX, shadowOffsetY, 0, 0 );
	//	}
	//}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	fillCircle
	@text	Draw a filled circle.
	
	@in		number x
	@in		number y
	@in		number r
	@in		number steps
	@out	nil
*/
int MOAIDraw::_fillCircle ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	float x0	= state.GetValue < float >( 1, 0.0f );
	float y0	= state.GetValue < float >( 2, 0.0f );
	float r		= state.GetValue < float >( 3, 0.0f );
	u32 steps	= state.GetValue < u32 >( 4, DEFAULT_ELLIPSE_STEPS );

	self->Get ().DrawEllipseFill ( x0, y0, r, r, steps );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	fillEllipse
	@text	Draw a filled ellipse.
	
	@in		number x
	@in		number y
	@in		number xRad
	@in		number yRad
	@in		number steps
	@out	nil
*/
int MOAIDraw::_fillEllipse ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	float x		= state.GetValue < float >( 1, 0.0f );
	float y		= state.GetValue < float >( 2, 0.0f );
	float xRad	= state.GetValue < float >( 3, 0.0f );
	float yRad	= state.GetValue < float >( 4, 0.0f );
	
	u32 steps = state.GetValue < u32 >( 5, DEFAULT_ELLIPSE_STEPS );

	self->Get ().DrawEllipseFill ( x, y, xRad, yRad, steps );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	fillFan
	@text	Draw a filled fan.
	
	@in		... vertices		List of vertices (x, y) or an array of vertices
								{ x0, y0, x1, y1, ... , xn, yn }
	@out	nil
*/
int MOAIDraw::_fillFan ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )

	if ( lua_istable ( L, -1 ) ) {
		self->DrawLuaArray( L, ZGL_PRIM_TRIANGLE_FAN );
	}
	else {
		self->DrawLuaParams( L, ZGL_PRIM_TRIANGLE_FAN );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	fillRect
	@text	Draw a filled rectangle.
	
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@out	nil
*/
int MOAIDraw::_fillRect ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	float x0 = state.GetValue < float >( 1, 0.0f );
	float y0 = state.GetValue < float >( 2, 0.0f );
	float x1 = state.GetValue < float >( 3, 0.0f );
	float y1 = state.GetValue < float >( 4, 0.0f );

	self->Get ().DrawRectFill ( x0, y0, x1, y1 );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setBlendMode
	@text	Sets the blend mode for drawing.
	
	@overload	Reset the blend mode to MOAIGraphicsProp.BLEND_NORMAL (equivalent to src = GL_ONE, dst = GL_ONE_MINUS_SRC_ALPHA). This will reset the blend function to GL_FUNC_ADD.

		@in		nil
		@out	nil

	@overload	Set blend mode using one of the Moai presets. This will reset the blend function to GL_FUNC_ADD.

		@in		number mode					One of MOAIGraphicsProp.BLEND_NORMAL, MOAIGraphicsProp.BLEND_ADD, MOAIGraphicsProp.BLEND_MULTIPLY.
		@out	nil
	
	@overload	Set blend mode using OpenGL source and dest factors. OpenGl blend factor constants are exposed as members of MOAIGraphicsProp.
				See the OpenGL documentation for an explanation of blending constants.

		@in		number srcFactor
		@in		number dstFactor
		@in		number equation
		@out	nil
*/
int MOAIDraw::_setBlendMode ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	if ( state.IsType ( 1, LUA_TNUMBER )) {
		if ( state.IsType ( 2, LUA_TNUMBER )) {

			u32 srcFactor = state.GetValue < u32 >( 1, 0 );
			u32 dstFactor = state.GetValue < u32 >( 2, 0 );
			u32 equation = state.GetValue < u32 >( 3, 0 );
			gfxMgr.mGfxState.SetBlendMode ( srcFactor, dstFactor, equation );
		}
		else {

			u32 blendModeID = state.GetValue < u32 >( 1, MOAIBlendMode::BLEND_NORMAL );
			
			MOAIBlendMode blendMode;
			blendMode.SetBlend ( blendModeID );
			gfxMgr.mGfxState.SetBlendMode ( blendMode );
		}
	}
	else {
		gfxMgr.mGfxState.SetBlendMode();
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_setClearColor ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	u32 clearFlags = gfxMgr.mGfxState.GetClearFlags () & ~ZGL_CLEAR_COLOR_BUFFER_BIT;
	ZLColorVec clearColor ( 0.0f, 0.0f, 0.0f, 1.0f );

	MOAIColor* color = state.GetLuaObject < MOAIColor >( 1, true );
	if ( color ) {
		clearColor = *color;
		clearFlags |= ZGL_CLEAR_COLOR_BUFFER_BIT;
	}

	if ( state.GetTop () > 1 ) {
	
		float r = state.GetValue < float >( 1, 0.0f );
		float g = state.GetValue < float >( 2, 0.0f );
		float b = state.GetValue < float >( 3, 0.0f );
		float a = state.GetValue < float >( 4, 1.0f );
		
		clearColor.Set ( r, g, b, a );
		clearFlags |= ZGL_CLEAR_COLOR_BUFFER_BIT;
	}
	
	gfxMgr.mGfxState.SetClearColor ( clearColor );
	gfxMgr.mGfxState.SetClearFlags ( clearFlags );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_setClearDepth ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	u32 clearFlags = gfxMgr.mGfxState.GetClearFlags () & ~ZGL_CLEAR_DEPTH_BUFFER_BIT;
	double clearDepth = 0.0f;
	
	if ( state.IsType ( 1, LUA_TNUMBER )) {
	
		clearDepth = state.GetValue < double >( 1, 0.0 );
		clearFlags |= ZGL_CLEAR_DEPTH_BUFFER_BIT;
	}
	
	gfxMgr.mGfxState.SetClearDepth ( clearDepth );
	gfxMgr.mGfxState.SetClearFlags ( clearFlags );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDefaultTexture
	@text	Specify a fallback texture to use when textures are
			unavailable (pending load, missing or in error state).
	
	@in		MOAITexture texture
	@out	MOAITexture texture		Texture that was passed in or created.
*/
int MOAIDraw::_setDefaultTexture ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	MOAITexture* texture = state.GetLuaObject < MOAITexture >( 1, false );
	
	if ( !texture ) {
		texture = new MOAITexture ();
		if ( !texture->Init ( state, 1 )) {
			// TODO: report error
			delete texture;
			texture = 0;
		}
	}

	gfxMgr.mGfxState.SetDefaultTexture ( gfxMgr, texture );

	if ( texture ) {
		texture->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_setMatrix ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	u32 matrixID = state.GetValue < u32 >( 1, MOAIGfxGlobalsCache::MODEL_TO_WORLD_MTX );

	if ( gfxMgr.mGfxState.IsInputMtx( matrixID )) {
	
		if ( state.IsType ( 2, LUA_TUSERDATA )) {
		
			MOAITransformBase* transform = state.GetLuaObject < MOAITransformBase >( 2, true );
			if ( transform ) {
				gfxMgr.mGfxState.SetMtx ( matrixID, transform->GetLocalToWorldMtx ());
			}
		}
		else {
			int size = state.GetTop () - 2;
			ZLMatrix4x4 mtx = state.GetMatrix ( 2, size );
			gfxMgr.mGfxState.SetMtx ( matrixID, mtx );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setPenColor

	@in		number r
	@in		number g
	@in		number b
	@opt	number a	Default value is 1.
	@out	nil
*/
int MOAIDraw::_setPenColor ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )

	float r = state.GetValue < float >( 1, 1.0f );
	float g = state.GetValue < float >( 2, 1.0f );
	float b = state.GetValue < float >( 3, 1.0f );
	float a = state.GetValue < float >( 4, 1.0f );

	MOAIGfxMgr::Get ().mGfxState.SetPenColor ( r, g, b, a );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setPenWidth

	@in		number width
	@out	nil
*/
int MOAIDraw::_setPenWidth ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )

	float width = state.GetValue < float >( 1, 1.0f );
	MOAIGfxMgr::Get ().mGfxState.SetPenWidth ( width );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_setScissorRect ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )

	// TODO: fix this code duplication from _setViewRect
	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	if ( state.IsType ( 1, LUA_TUSERDATA )) {
		MOAIViewport* viewport = state.GetLuaObject < MOAIViewport >( 1, true );
		if ( viewport ) {
			gfxMgr.mGfxState.SetScissorRect ( *viewport );
		}
	}
	else {
		ZLRect rect;
		rect.Init ( 0, 0, gfxMgr.mGfxState.GetBufferWidth (), gfxMgr.mGfxState.GetBufferHeight ());
		rect = state.GetValue < ZLRect >( 1, rect );
		gfxMgr.mGfxState.SetScissorRect ( rect );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_setViewProj ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	MOAIViewport* viewport = state.GetLuaObject < MOAIViewport >( 1, false );
	MOAICamera* camera = state.GetLuaObject < MOAICamera >( 2, false );
	
	MOAIGfxMgr::Get ().mGfxState.SetViewProj ( viewport, camera );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_setViewRect ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )

	// TODO: fix this code duplication from _setScissorRect
	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	if ( state.IsType ( 1, LUA_TUSERDATA )) {
		MOAIViewport* viewport = state.GetLuaObject < MOAIViewport >( 1, true );
		if ( viewport ) {
			gfxMgr.mGfxState.SetViewRect ( *viewport );
		}
	}
	else {
		ZLRect rect;
		rect.Init ( 0, 0, gfxMgr.mGfxState.GetBufferWidth (), gfxMgr.mGfxState.GetBufferHeight ());
		rect = state.GetValue < ZLRect >( 1, rect );
		gfxMgr.mGfxState.SetViewRect ( rect );
	}
	return 0;
}

//================================================================//
// MOAIDraw
//================================================================//

//----------------------------------------------------------------//
bool MOAIDraw::Bind () {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	if ( !gfxMgr.mGfxState.SetTexture ()) return false;
	if ( !gfxMgr.mGfxState.SetShader ( MOAIShaderMgr::LINE_SHADER )) return false;
	gfxMgr.mGfxState.SetVertexFormat ( MOAIVertexFormatMgr::XYZWC );
	
	gfxMgr.mVertexCache.SetVertexTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::MODEL_TO_CLIP_MTX ));
	gfxMgr.mVertexCache.SetUVTransform ( MOAIGfxMgr::Get ().mGfxState.GetMtx ( MOAIGfxGlobalsCache::UV_TO_MODEL_MTX ));

	return true;
}

//----------------------------------------------------------------//
void MOAIDraw::DrawAnimCurve ( const MOAIAnimCurve& curve, u32 resolution ) {

	curve.Draw ( resolution );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawAxisGrid ( ZLVec2D loc, ZLVec2D vec, float size ) {

	ZLMatrix4x4 mtx = MOAIGfxMgr::Get ().mGfxState.GetMtx ( MOAIGfxGlobalsCache::WORLD_TO_CLIP_MTX );
	
	ZLMatrix4x4 invMtx;
	invMtx.Inverse ( mtx );
	
	// Set the axis to the grid length so we can get the length back post-transform
	vec.SetLength ( size );
	
	mtx.Transform ( loc );
	mtx.TransformVec ( vec );

	// Get the axis unit vector
	ZLVec2D norm = vec;
	size = norm.NormSafe ();
	
	// Get the axis normal
	ZLVec2D perpNorm ( norm.mY, -norm.mX );
	
	// Project the corners of the viewport onto the axis to get the mix/max bounds
	float dot;
	float min;
	float max;
	
	ZLVec2D corner;
	
	// left, top
	corner.Init ( -1.0f, 1.0f );
	corner.Sub ( loc );
	dot = norm.Dot ( corner );
	
	min = dot;
	max = dot;
	
	// right, top
	corner.Init ( 1.0f, 1.0f );
	corner.Sub ( loc );
	dot = norm.Dot ( corner );
	
	min = ( dot < min ) ? dot : min;
	max = ( dot > max ) ? dot : max;
	
	// right, bottom
	corner.Init ( 1.0f, -1.0f );
	corner.Sub ( loc );
	dot = norm.Dot ( corner );
	
	min = ( dot < min ) ? dot : min;
	max = ( dot > max ) ? dot : max;
	
	// left, bottom
	corner.Init ( -1.0f, -1.0f );
	corner.Sub ( loc );
	dot = norm.Dot ( corner );
	
	min = ( dot < min ) ? dot : min;
	max = ( dot > max ) ? dot : max;
	
	// Get the start andstop grids
	s32 start = ( s32 )( min / size ) - 1;
	s32 stop = ( s32 )( max / size ) + 1;
	
	// Set the pen to the first...
	ZLVec2D pen = norm;
	pen.Scale (( float )start * size );
	pen.Add ( loc );
	
	// Step along the axis to draw perpendicular grid lines
	ZLRect viewRect;
	viewRect.Init ( -1.0f, -1.0f, 1.0f, 1.0f );
	
	for ( ; start < stop; ++start ) {
		
		ZLVec2D p0;
		ZLVec2D p1;
		
		if ( viewRect.GetIntersection ( pen, perpNorm, p0, p1 )) {
			
			invMtx.Transform ( p0 );
			invMtx.Transform ( p1 );
			
			this->DrawLine ( p0, p1 );
		}
		
		pen.Add ( vec );
	}
}

//----------------------------------------------------------------//
void MOAIDraw::DrawBezierCurve ( const ZLCubicBezier2D& bezier ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	MOAIDrawVertexLineStripWriter2D writer;
	
	writer.Begin ();
		bezier.Flatten ( writer );
	writer.End ();
}

//----------------------------------------------------------------//
void MOAIDraw::DrawElements ( MOAIGfxBuffer* vtxBuffer, MOAIVertexFormat* vtxFormat, u32 count ) {

	// TODO: fix this?

	UNUSED ( vtxBuffer );
	UNUSED ( vtxFormat );
	UNUSED ( count );
	
//	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
//	
//	MOAIGfxMgr::Get ().UnbindBufferedDrawing ();
//	
//	vtxBuffer->Bind ();
//	vtxFormat->Bind ( 0 );
//	
//	gfxMgr.SetVertexMtxMode ( MOAIGfxMgr::VTX_STAGE_MODEL, MOAIGfxMgr::VTX_STAGE_MODEL );
//	
//	gfxMgr.UpdateShaderGlobals ();
//		
//	// TODO: use gfxMgr to cache buffers
//
//	zglDrawArrays ( ZGL_PRIM_TRIANGLES, 0, count );
//	vtxBuffer->Unbind ();
//	vtxFormat->Unbind ();
}

//----------------------------------------------------------------//
void MOAIDraw::DrawGrid ( const ZLRect& rect, u32 xCells, u32 yCells ) {

	if ( xCells > 1 ) {
		float xStep = rect.Width () / ( float )xCells;
		for ( u32 i = 1; i < xCells; ++i ) {
			float x = rect.mXMin + (( float )i * xStep );
			ZLVec2D v0 ( x, rect.mYMin );
			ZLVec2D v1 ( x, rect.mYMax );
			
			this->DrawLine ( v0, v1 );
		}
	}

	if ( yCells > 1 ) {
		float yStep = rect.Height () / ( float )yCells;
		for ( u32 i = 1; i < yCells; ++i ) {
			float y = rect.mYMin + (( float )i * yStep );
			ZLVec2D v0 ( rect.mXMin, y );
			ZLVec2D v1 ( rect.mXMax, y );
			
			this->DrawLine ( v0, v1 );
		}
	}

	this->DrawRectOutline ( rect );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawLuaParams ( lua_State* L, u32 primType ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	MOAILuaState state ( L );

	u32 total = state.GetTop () >> 1;
	
	gfxMgr.mVertexCache.BeginPrim ( primType, total );
	
	for ( u32 i = 0; i < total; ++i ) {
		
		u32 idx = ( i << 1 ) + 1;
		
		float x = state.GetValue < float >( idx, 0.0f );
		float y = state.GetValue < float >( idx + 1, 0.0f );
		
		gfxMgr.mVertexCache.WriteVtx ( x, y, 0.0f );
		gfxMgr.mVertexCache.WritePenColor4b ();
	}
	
	gfxMgr.mVertexCache.EndPrim ();
}


//----------------------------------------------------------------//
void MOAIDraw::DrawLuaArray ( lua_State* L, u32 primType ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	MOAILuaState state ( L );

	float x = 0.0f;
	float y = 0.0f;
	
	size_t size = state.GetTableSize ( -1 ) >> 2;
	
	gfxMgr.mVertexCache.BeginPrim ( primType, size );

	u32 counter = 0;
	lua_pushnil ( L );
    while ( lua_next ( L, 1 ) != 0 ) {
		// Assuming odd-numbered array entries to be x-coordinate (abscissa),
		// even-numbered array entries to be y-coordinate (oordinate).
		if ( counter % 2 == 0 ) {
			x = state.GetValue < float >( -1, 0.0f );
		} else {
			y = state.GetValue < float >( -1, 0.0f );
			gfxMgr.mVertexCache.WriteVtx ( x, y );
			gfxMgr.mVertexCache.WritePenColor4b ();
		}
		++counter;
		lua_pop ( L, 1 );
	}

	gfxMgr.mVertexCache.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIDraw::DrawRay ( float x, float y, float dx, float dy ) {
	
	ZLVec2D loc ( x, y );
	ZLVec2D vec ( dx, dy );
	
	ZLMatrix4x4 mtx = MOAIGfxMgr::Get ().mGfxState.GetMtx ( MOAIGfxGlobalsCache::WORLD_TO_CLIP_MTX );
	
	ZLMatrix4x4 invMtx;
	invMtx.Inverse ( mtx );
	
	mtx.Transform ( loc );
	mtx.TransformVec ( vec );
	
	ZLRect viewRect;
	viewRect.Init ( -1.0f, -1.0f, 1.0f, 1.0f );
	
	ZLVec2D p0;
	ZLVec2D p1;
	
	if ( viewRect.GetIntersection ( loc, vec, p0, p1 )) {
		
		invMtx.Transform ( p0 );
		invMtx.Transform ( p1 );
		
		MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
		
		gfxMgr.mVertexCache.BeginPrim ( ZGL_PRIM_LINES, 2 );
		
			gfxMgr.mVertexCache.WriteVtx ( p0.mX, p0.mY, 0.0f );
			gfxMgr.mVertexCache.WritePenColor4b ();
			
			gfxMgr.mVertexCache.WriteVtx ( p1.mX, p1.mY, 0.0f );
			gfxMgr.mVertexCache.WritePenColor4b ();

		gfxMgr.mVertexCache.EndPrim ();
	}
}

//----------------------------------------------------------------//
void MOAIDraw::DrawTexture ( float left, float top, float right, float bottom, MOAITexture* texture ) {
	
	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	if ( texture ) {

//		gfxMgr.SetBlendMode ( ZGL_BLEND_FACTOR_ONE, ZGL_BLEND_FACTOR_ZERO );
		if ( !gfxMgr.mGfxState.SetTexture ( texture )) return;
		if ( !gfxMgr.mGfxState.SetShader ( MOAIShaderMgr::DECK2D_SHADER )) return;

		const ZLColorVec& orgColor = gfxMgr.mGfxState.GetPenColor ();
		gfxMgr.mGfxState.SetPenColor ( 1, 1, 1, 1 );
		
		MOAIQuadBrush::BindVertexFormat ();

		MOAIQuadBrush quad;
		quad.SetVerts ( left, top, right, bottom );
		quad.SetUVs ( 0, 0, 1, 1 );		
		quad.Draw ();
		
//		gfxMgr.SetBlendMode ();
		gfxMgr.mGfxState.SetPenColor ( orgColor );
		
		MOAIDraw::Bind ();
	}
}

//----------------------------------------------------------------//
void MOAIDraw::DrawVertexArray ( const ZLVec3D* verts, u32 count, u32 color, u32 primType ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	gfxMgr.mGfxState.SetPenColor ( color );
	gfxMgr.mVertexCache.BeginPrim ( primType, count );
	
	for ( u32 i = 0; i < count; ++i ) {
		const ZLVec3D& vtx = verts [ i ];
		gfxMgr.mVertexCache.WriteVtx ( vtx );
		gfxMgr.mVertexCache.WritePenColor4b ();
	}

	gfxMgr.mVertexCache.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIDraw::DrawVertexArray2D ( const float* verts, u32 count, u32 color, u32 primType ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	gfxMgr.mGfxState.SetPenColor ( color );
	gfxMgr.mVertexCache.BeginPrim ( primType, count );
	
	for ( u32 i = 0; i < count; ++i ) {
		u32 v = i << 1;
		gfxMgr.mVertexCache.WriteVtx ( verts [ v ], verts [ v + 1 ], 0.0f );
		gfxMgr.mVertexCache.WritePenColor4b ();
	}

	gfxMgr.mVertexCache.EndPrim ();
}

//----------------------------------------------------------------//
MOAIDraw::MOAIDraw () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIDraw::~MOAIDraw () {
}

//----------------------------------------------------------------//
void MOAIDraw::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );

	state.SetField ( -1, "PROJ_MATRIX",					( u32 )MOAIGfxGlobalsCache::VIEW_TO_CLIP_MTX );
	state.SetField ( -1, "UV_MATRIX",					( u32 )MOAIGfxGlobalsCache::UV_TO_MODEL_MTX );
	state.SetField ( -1, "VIEW_MATRIX",					( u32 )MOAIGfxGlobalsCache::WORLD_TO_VIEW_MTX );
	state.SetField ( -1, "WORLD_MATRIX",				( u32 )MOAIGfxGlobalsCache::MODEL_TO_WORLD_MTX );

	luaL_Reg regTable [] = {
		{ "bindFrameBuffer",		_bindFrameBuffer },
		{ "bindIndexBuffer",		_bindIndexBuffer },
		{ "bindShader",				_bindShader },
		{ "bindTexture",			_bindTexture },
		{ "bindVertexArray",		_bindVertexArray },
		{ "bindVertexBuffer",		_bindVertexBuffer },
		{ "bindVertexFormat",		_bindVertexFormat },
		{ "clear",					_clear },
		{ "drawAnimCurve",			_drawAnimCurve },
		//{ "drawAxisGrid",			_drawAxisGrid }, // TODO
		{ "drawBezierCurve",		_drawBezierCurve },
		{ "drawBoxOutline",			_drawBoxOutline },
		{ "drawCircle",				_drawCircle },
		{ "drawCircleSpokes",		_drawCircleSpokes },
		{ "drawElements",			_drawElements },
		{ "drawEllipse",			_drawEllipse },
		{ "drawEllipseSpokes",		_drawEllipseSpokes },
		//{ "drawGrid",				_drawGrid }, // TODO
		{ "drawLine",				_drawLine },
		{ "drawPoints",				_drawPoints },
		{ "drawRay",				_drawRay },
		{ "drawRect",				_drawRect },
		{ "drawText",				_drawText },
		{ "drawTexture",			_drawTexture },
		{ "fillCircle",				_fillCircle },
		{ "fillEllipse",			_fillEllipse },
		{ "fillFan",				_fillFan },
		{ "fillRect",				_fillRect },
		{ "setBlendMode",			_setBlendMode },
		{ "setClearColor",			_setClearColor },
		{ "setClearDepth",			_setClearDepth },
		{ "setDefaultTexture",		_setDefaultTexture },
		{ "setMatrix",				_setMatrix },
		{ "setPenColor",			_setPenColor },
		{ "setPenWidth",			_setPenWidth },
		{ "setScissorRect",			_setScissorRect },
		{ "setViewProj",			_setViewProj },
		{ "setViewRect",			_setViewRect },
		
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
