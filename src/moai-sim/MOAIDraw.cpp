// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAbstractChildTransform.h>
#include <moai-sim/MOAIAnimCurveFloat.h>
#include <moai-sim/MOAIDraw.h>
#include <moai-sim/MOAIFont.h>
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
	MOAIGfxMgr&	mGfxMgr;

public:

	//----------------------------------------------------------------//
	void Begin () {
	
		this->mVertexCount = 0;
		this->mGfxMgr.BeginPrim ( MOAIGfxTopologyEnum::LINE_STRIP, 1 );
	}
	
	//----------------------------------------------------------------//
	MOAIDrawVertexLineStripWriter2D () :
		mGfxMgr ( MOAIGfxMgr::Get ()) {
	}

	//----------------------------------------------------------------//
	void WriteVertex ( const ZLVec2D& v ) {
		
		if (( this->mVertexCount > 0 ) && ( this->mGfxMgr.ContinuePrim ( 1 ) == MOAIGfxMgr::CONTINUE_ROLLOVER )) {

			// if we roll over, repeat the last vertex to start a new line strip
			this->mGfxMgr.WriteVtx ( this->mLastVertex.mX, this->mLastVertex.mY );
			this->mGfxMgr.WritePenColor4b ();
		}
		
		this->mGfxMgr.WriteVtx ( v.mX, v.mY );
		this->mGfxMgr.WritePenColor4b ();
		
		this->mLastVertex = v;
	}
	
	//----------------------------------------------------------------//
	void End () {
	
		this->mGfxMgr.EndPrim ();
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
	
	const ZLMatrix4x4& orgWorldTransform = gfxMgr.GetMtx ( MOAIGfxMgr::MODEL_TO_WORLD_MTX );
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
				MOAITexture* glyphTexture = font.GetGlyphTexture ( *glyph );
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
	//const ZLMatrix4x4& orgWorldTransform = gfxState.GetMtx ( MOAIGfxMgr::MODEL_TO_WORLD_MTX );

	// TODO
	//GLint orgSrcBlend, orgDestBlend;
	//glGetIntegerv ( GL_BLEND_SRC, &orgSrcBlend );
	//glGetIntegerv ( GL_BLEND_DST, &orgDestBlend );

	// Apply render state
	gfxMgr.SetShader ( MOAIShaderPresetEnum::FONT_SHADER );
	
	gfxMgr.SetBlendMode ( MOAIBlendFactorEnum::ONE, MOAIBlendFactorEnum::ONE_MINUS_SRC_ALPHA );
	gfxMgr.SetVertexTransform ( MOAIGfxMgr::MODEL_TO_WORLD_MTX );
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
	ZLColorVec penColor = gfxMgr.GetPenColor ();
	if ( drawDropShadows ) {

		numPasses = 2;		
		gfxMgr.SetPenColor ( 0, 0, 0, 1 );
		//offsetX = shadowOffsetX;
		//offsetY = shadowOffsetY;
	}

	for ( u32 pass = 0; pass < numPasses; pass++ ) {

		if ( pass == 1 || numPasses == 1 ) {
			gfxMgr.SetPenColor ( penColor );
			//offsetX = 0;
			//offsetY = 0;
		}

		//STLList < GlyphPlacement >::const_iterator it;
		//for ( it = g_CurrentTextDrawContext->mGlyphs.begin (); it != g_CurrentTextDrawContext->mGlyphs.end (); ++it ) {

		//	const GlyphPlacement& glyphPlacement = *it;
		//	MOAIGlyph* glyph = glyphPlacement.glyph;
		//	MOAITexture* glyphTexture = font.GetGlyphTexture ( *glyph );
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
int MOAIDraw::_bind ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	self->Bind ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_bindFrameBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	MOAIGfxMgr::Get ().SetFrameBuffer ( state.GetLuaObject < MOAIFrameBuffer >( 1, false ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_bindIndexBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	MOAIGfxMgr::Get ().SetIndexBuffer ( state.GetLuaObject < MOAIIndexBuffer >( 1, false ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_bindShader ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	MOAIShader* shader = MOAIGfxMgr::Get ().AffirmShader ( state, 1 );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_bindTexture ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	MOAIGfxMgr::Get ().SetTexture ( state.GetLuaObject < MOAITexture >( 1, false ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_bindVertexArray ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	MOAIGfxMgr::Get ().SetVertexArray ( state.GetLuaObject < MOAIVertexArray >( 1, false ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_bindVertexBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	MOAIGfxMgr::Get ().SetVertexBuffer ( state.GetLuaObject < MOAIVertexBuffer >( 1, false ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_bindVertexFormat ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	switch ( lua_type ( state, 1 )) {
	
		case LUA_TUSERDATA:
			MOAIGfxMgr::Get ().SetVertexFormat ( state.GetLuaObject < MOAIVertexFormat >( 1, true ));
			break;
		
		case LUA_TNUMBER: {
			MOAIVertexFormat* vertexFormat = MOAIGfxMgr::Get ().GetVertexFormatPreset (( MOAIVertexFormatPresetEnum )state.GetValue < u32 >( 1, ( u32 )MOAIVertexFormatPresetEnum::XYZWC ));
			MOAIGfxMgr::Get ().SetVertexFormat ( vertexFormat );
			break;
		}
		default:
			MOAIGfxMgr::Get ().SetVertexFormat ();
			break;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_clear ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )

	MOAIGfxMgr::Get ().ClearSurface ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_drawAnimCurve ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )

	MOAIAnimCurveFloat* curve	= state.GetLuaObject < MOAIAnimCurveFloat >( 1, true );
	u32 resolution				= state.GetValue < u32 >( 2, 1 );

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
	
	MOAIGfxBufferGL* vtxBuffer = state.GetLuaObject < MOAIGfxBufferGL >( 1, false );
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
		self->DrawLuaArray ( L, MOAIGfxTopologyEnum::LINE_STRIP );
	}
	else {
		self->DrawLuaParams ( L, MOAIGfxTopologyEnum::LINE_STRIP );
	}	
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
		self->DrawLuaArray ( L, MOAIGfxTopologyEnum::POINT_LIST );
	}
	else {
		self->DrawLuaParams ( L, MOAIGfxTopologyEnum::POINT_LIST );
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
	
	MOAITexture* texture = MOAIGfxMgr::Get ().AffirmTexture ( state, 5 );

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
		self->DrawLuaArray( L, MOAIGfxTopologyEnum::TRIANGLE_FAN );
	}
	else {
		self->DrawLuaParams( L, MOAIGfxTopologyEnum::TRIANGLE_FAN );
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
// TODO: doxygen
int MOAIDraw::_fillRoundedRect ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	float x0 = state.GetValue < float >( 1, 0.0f );
	float y0 = state.GetValue < float >( 2, 0.0f );
	float x1 = state.GetValue < float >( 3, 0.0f );
	float y1 = state.GetValue < float >( 4, 0.0f );
	
	float xRad		= state.GetValue < float >( 5, 0.0f );
	float yRad		= state.GetValue < float >( 6, xRad );
	u32 steps		= state.GetValue < u32 >( 7, DEFAULT_ELLIPSE_STEPS );

	self->Get ().DrawRoundedRectFill ( x0, y0, x1, y1, xRad, yRad, steps );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_getPenColor ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )

	ZLColorVec color = MOAIGfxMgr::Get ().GetPenColor ();
	
	state.Push ( color.mR );
	state.Push ( color.mG );
	state.Push ( color.mB );
	state.Push ( color.mA );
	
	return 4;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_popGfxState ( lua_State* L ) {

	MOAIGfxMgr::Get ().PopState ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_pushGfxState ( lua_State* L ) {

	MOAIGfxMgr::Get ().PushState ();
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setBlendMode
	@text	Set blend mode using OpenGL source and dest factors. OpenGl blend factor constants are exposed as members of MOAIGraphicsProp.
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

			MOAIBlendFactorEnum::_ srcFactor		= ( MOAIBlendFactorEnum::_ )state.GetValue < u32 >( 1, 0 );
			MOAIBlendFactorEnum::_ dstFactor		= ( MOAIBlendFactorEnum::_ )state.GetValue < u32 >( 2, 0 );
			MOAIBlendFuncEnum::_ equation			= ( MOAIBlendFuncEnum::_ )state.GetValue < u32 >( 3, 0 );
			gfxMgr.SetBlendMode ( srcFactor, dstFactor, equation );
		}
	}
	else {
		gfxMgr.SetBlendMode ();
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_setClearColor ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	u32 clearFlags = gfxMgr.GetClearFlags () & ~MOAIClearFlagsEnum::COLOR_BUFFER_BIT;
	ZLColorVec clearColor ( 0.0f, 0.0f, 0.0f, 1.0f );

	MOAIColor* color = state.GetLuaObject < MOAIColor >( 1, true );
	if ( color ) {
		clearColor = *color;
		clearFlags |= MOAIClearFlagsEnum::COLOR_BUFFER_BIT;
	}

	if ( state.GetTop () > 1 ) {
	
		float r = state.GetValue < float >( 1, 0.0f );
		float g = state.GetValue < float >( 2, 0.0f );
		float b = state.GetValue < float >( 3, 0.0f );
		float a = state.GetValue < float >( 4, 1.0f );
		
		clearColor.Set ( r, g, b, a );
		clearFlags |= MOAIClearFlagsEnum::COLOR_BUFFER_BIT;
	}
	
	gfxMgr.SetClearColor ( clearColor );
	gfxMgr.SetClearFlags ( clearFlags );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_setClearDepth ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	u32 clearFlags = gfxMgr.GetClearFlags () & ~MOAIClearFlagsEnum::DEPTH_BUFFER_BIT;
	double clearDepth = 0.0f;
	
	if ( state.IsType ( 1, LUA_TNUMBER )) {
	
		clearDepth = state.GetValue < double >( 1, 0.0 );
		clearFlags |= MOAIClearFlagsEnum::DEPTH_BUFFER_BIT;
	}
	
	gfxMgr.SetClearDepth ( clearDepth );
	gfxMgr.SetClearFlags ( clearFlags );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setCullMode
 @text	Set cull mode
 See the OpenGL documentation for an explanation of culling modes.
 
 @in	number
 @out	nil
 */
int MOAIDraw::_setCullMode ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	if ( state.IsType ( 1, LUA_TNUMBER )) {
			
		MOAICullFuncEnum::_ cullMode = ( MOAICullFuncEnum::_ )state.GetValue < u32 >( 1, 0 );
		gfxMgr.SetCullFunc ( cullMode );
	}
	else {
		gfxMgr.SetCullFunc ();
	}
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

	MOAITexture* texture = gfxMgr.AffirmTexture ( state, 1 );
	gfxMgr.SetDefaultTexture ( texture );

	if ( texture ) {
		texture->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDepthFunc
 @text	Set depth function.
 
 @in	number
 @out	nil
 */
int MOAIDraw::_setDepthFunc ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	if ( state.IsType ( 1, LUA_TNUMBER )) {
		
		MOAIDepthFuncEnum::_ depthFunc = ( MOAIDepthFuncEnum::_ )state.GetValue < u32 >( 1, 0 );
		gfxMgr.SetDepthFunc ( depthFunc );
	}
	else {
		gfxMgr.SetDepthFunc ();
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_setMatrix ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	u32 matrixID = state.GetValue < u32 >( 1, MOAIGfxMgr::MODEL_TO_WORLD_MTX );

	if ( gfxMgr.IsInputMtx( matrixID )) {
	
		if ( state.IsType ( 2, LUA_TUSERDATA )) {
		
			MOAIAbstractChildTransform* transform = state.GetLuaObject < MOAIAbstractChildTransform >( 2, true );
			if ( transform ) {
				gfxMgr.SetMtx ( matrixID, transform->GetLocalToWorldMtx ());
			}
		}
		else {
			int size = state.GetTop () - 2;
			ZLMatrix4x4 mtx = state.GetMatrix ( 2, size );
			gfxMgr.SetMtx ( matrixID, mtx );
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

	MOAIGfxMgr::Get ().SetPenColor ( r, g, b, a );
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
	MOAIGfxMgr::Get ().SetPenWidth ( width );
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
			gfxMgr.SetScissorRect ( *viewport );
		}
	}
	else {
		ZLRect rect ( 0.0, 0.0, (float) gfxMgr.GetBufferWidth (), (float) gfxMgr.GetBufferHeight ());
		rect = state.GetValue < ZLRect >( 1, rect );
		gfxMgr.SetScissorRect ( rect );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_setViewProj ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	MOAIViewport* viewport = state.GetLuaObject < MOAIViewport >( 1, false );
	MOAICamera* camera = state.GetLuaObject < MOAICamera >( 2, false );
	
	MOAIGfxMgr::Get ().SetViewProj ( viewport, camera );
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
			gfxMgr.SetViewRect ( *viewport );
		}
	}
	else {
		ZLRect rect ( 0.0, 0.0, ( float )gfxMgr.GetBufferWidth (), ( float )gfxMgr.GetBufferHeight ());
		rect = state.GetValue < ZLRect >( 1, rect );
		gfxMgr.SetViewRect ( rect );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_strokeRoundedRect ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDraw, "" )
	
	float x0 = state.GetValue < float >( 1, 0.0f );
	float y0 = state.GetValue < float >( 2, 0.0f );
	float x1 = state.GetValue < float >( 3, 0.0f );
	float y1 = state.GetValue < float >( 4, 0.0f );
	
	float stroke	= state.GetValue < float >( 5, 0.0f );
	float offset	= state.GetValue < float >( 6, 0.0f );
	
	float xRad		= state.GetValue < float >( 7, 0.0f );
	float yRad		= state.GetValue < float >( 8, xRad );
	u32 steps		= state.GetValue < u32 >( 9, DEFAULT_ELLIPSE_STEPS );

	self->Get ().DrawRoundedRectStroke ( x0, y0, x1, y1, xRad, yRad, steps, stroke, offset );
	return 0;
}

//================================================================//
// MOAIDraw
//================================================================//

//----------------------------------------------------------------//
bool MOAIDraw::Bind () {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	gfxMgr.SetTexture ();
	gfxMgr.SetShader ( MOAIShaderPresetEnum::LINE_SHADER );
	gfxMgr.SetVertexFormat ( MOAIVertexFormatPresetEnum::XYZWC );
	
	gfxMgr.SetVertexTransform ( MOAIGfxMgr::MODEL_TO_CLIP_MTX );
	gfxMgr.SetUVTransform ( MOAIGfxMgr::UV_TO_MODEL_MTX );

	gfxMgr.SetCullFunc (); // no culling in immediate mode drawing

	return true;
}

//----------------------------------------------------------------//
void MOAIDraw::DrawAnimCurve ( const MOAIAnimCurveFloat& curve, u32 resolution ) {

	curve.Draw ( resolution );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawAxisGrid ( ZLVec2D loc, ZLVec2D vec, float size ) {

	ZLMatrix4x4 mtx = MOAIGfxMgr::Get ().GetMtx ( MOAIGfxMgr::WORLD_TO_CLIP_MTX );
	
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

	MOAIDrawVertexLineStripWriter2D writer;
	
	writer.Begin ();
		bezier.Flatten ( writer );
	writer.End ();
}

//----------------------------------------------------------------//
void MOAIDraw::DrawElements ( MOAIGfxBufferGL* vtxBuffer, MOAIVertexFormat* vtxFormat, u32 count ) {

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
//	zglDrawArrays ( MOAIGfxTopologyEnum::TRIANGLE_LIST, 0, count );
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
void MOAIDraw::DrawLuaParams ( lua_State* L, MOAIGfxTopologyEnum::_ primType ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	MOAILuaState state ( L );

	u32 total = state.GetTop () >> 1;
	
	gfxMgr.BeginPrim ( primType, total );
	
	for ( u32 i = 0; i < total; ++i ) {
		
		u32 idx = ( i << 1 ) + 1;
		
		float x = state.GetValue < float >( idx, 0.0f );
		float y = state.GetValue < float >( idx + 1, 0.0f );
		
		gfxMgr.WriteVtx ( x, y, 0.0f );
		gfxMgr.WritePenColor4b ();
	}
	
	gfxMgr.EndPrim ();
}


//----------------------------------------------------------------//
void MOAIDraw::DrawLuaArray ( lua_State* L, MOAIGfxTopologyEnum::_ primType ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	MOAILuaState state ( L );

	float x = 0.0f;
	float y = 0.0f;
	
	size_t size = state.GetTableSize ( -1 ) >> 2;
	
	gfxMgr.BeginPrim ( primType, size );

	u32 counter = 0;
	lua_pushnil ( L );
    while ( lua_next ( L, 1 ) != 0 ) {
		// Assuming odd-numbered array entries to be x-coordinate (abscissa),
		// even-numbered array entries to be y-coordinate (oordinate).
		if ( counter % 2 == 0 ) {
			x = state.GetValue < float >( -1, 0.0f );
		} else {
			y = state.GetValue < float >( -1, 0.0f );
			gfxMgr.WriteVtx ( x, y );
			gfxMgr.WritePenColor4b ();
		}
		++counter;
		lua_pop ( L, 1 );
	}

	gfxMgr.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIDraw::DrawRay ( float x, float y, float dx, float dy ) {
	
	ZLVec2D loc ( x, y );
	ZLVec2D vec ( dx, dy );
	
	ZLMatrix4x4 mtx = MOAIGfxMgr::Get ().GetMtx ( MOAIGfxMgr::WORLD_TO_CLIP_MTX );
	
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
		
		gfxMgr.BeginPrim ( MOAIGfxTopologyEnum::LINE_LIST, 2 );
		
			gfxMgr.WriteVtx ( p0.mX, p0.mY, 0.0f );
			gfxMgr.WritePenColor4b ();
			
			gfxMgr.WriteVtx ( p1.mX, p1.mY, 0.0f );
			gfxMgr.WritePenColor4b ();

		gfxMgr.EndPrim ();
	}
}

//----------------------------------------------------------------//
void MOAIDraw::DrawTexture ( float left, float top, float right, float bottom, MOAITexture* texture ) {
	
	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	if ( texture && texture->IsReadyForUse ()) {

//		gfxMgr.SetBlendMode ( MOAIBlendFactorEnum::ONE, MOAIBlendFactorEnum::ZERO );
		gfxMgr.SetTexture ( texture );
		gfxMgr.SetShader ( MOAIShaderPresetEnum::DECK2D_SHADER );

		const ZLColorVec& orgColor = gfxMgr.GetPenColor ();
		gfxMgr.SetPenColor ( 1, 1, 1, 1 );
		
		MOAIQuadBrush::BindVertexFormat ();

		MOAIQuadBrush quad;
		quad.SetVerts ( left, top, right, bottom );
		quad.SetUVs ( 0, 0, 1, 1 );		
		quad.Draw ();
		
//		gfxMgr.SetBlendMode ();
		gfxMgr.SetPenColor ( orgColor );
		
		MOAIDraw::Bind ();
	}
}

//----------------------------------------------------------------//
void MOAIDraw::DrawVertexArray ( const ZLVec3D* verts, u32 count, u32 color, MOAIGfxTopologyEnum::_ primType ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	gfxMgr.SetPenColor ( color );
	gfxMgr.BeginPrim ( primType, count );
	
	for ( u32 i = 0; i < count; ++i ) {
		const ZLVec3D& vtx = verts [ i ];
		gfxMgr.WriteVtx ( vtx );
		gfxMgr.WritePenColor4b ();
	}

	gfxMgr.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIDraw::DrawVertexArray2D ( const float* verts, u32 count, u32 color, MOAIGfxTopologyEnum::_ primType ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	gfxMgr.SetPenColor ( color );
	gfxMgr.BeginPrim ( primType, count );
	
	for ( u32 i = 0; i < count; ++i ) {
		u32 v = i << 1;
		gfxMgr.WriteVtx ( verts [ v ], verts [ v + 1 ], 0.0f );
		gfxMgr.WritePenColor4b ();
	}

	gfxMgr.EndPrim ();
}

//----------------------------------------------------------------//
MOAIDraw::MOAIDraw () {

	MOAI_LUA_OBJECT_RTTI_SINGLE ( MOAIDraw, MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIDraw::~MOAIDraw () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIDraw::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	UNUSED ( state );

	state.SetField ( -1, "PROJ_MATRIX",					( u32 )MOAIGfxMgr::VIEW_TO_CLIP_MTX );
	state.SetField ( -1, "UV_MATRIX",					( u32 )MOAIGfxMgr::UV_TO_MODEL_MTX );
	state.SetField ( -1, "VIEW_MATRIX",					( u32 )MOAIGfxMgr::WORLD_TO_VIEW_MTX );
	state.SetField ( -1, "WORLD_MATRIX",				( u32 )MOAIGfxMgr::MODEL_TO_WORLD_MTX );

	luaL_Reg regTable [] = {
		{ "bind",					_bind },
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
		{ "fillRoundedRect",		_fillRoundedRect },
		{ "getPenColor",			_getPenColor },
		{ "popGfxState",			_popGfxState },
		{ "pushGfxState",			_pushGfxState },
		{ "setBlendMode",			_setBlendMode },
		{ "setClearColor",			_setClearColor },
		{ "setClearDepth",			_setClearDepth },
		{ "setCullMode",			_setCullMode },
		{ "setDefaultTexture",		_setDefaultTexture },
		{ "setDepthFunc",			_setDepthFunc },
		{ "setMatrix",				_setMatrix },
		{ "setPenColor",			_setPenColor },
		{ "setPenWidth",			_setPenWidth },
		{ "setScissorRect",			_setScissorRect },
		{ "setViewProj",			_setViewProj },
		{ "setViewRect",			_setViewRect },
		{ "strokeRoundedRect",		_strokeRoundedRect },
		
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
