// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDraw.h>
#include <moai-sim/MOAIFont.h>

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
	MOAIDraw::BindVectorPresets ();

	//gfxMgr.SetVertexTransform ( MOAIGfxMgr::VTX_WORLD_TRANSFORM, orgWorldTransform );
	//gfxMgr.SetBlendMode ( orgSrcBlend, orgDestBlend ); // TODO

	// Clear context
	g_CurrentTextDrawContext->mFont = 0;
	g_CurrentTextDrawContext->mGlyphs.clear();
	g_CurrentTextDrawContext = 0;
}

//================================================================//
// MOAIDraw
//================================================================//

//----------------------------------------------------------------//
MOAIDraw::MOAIDraw () {
}

//----------------------------------------------------------------//
MOAIDraw::~MOAIDraw () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIDraw::MOAIAbstractDrawingAPI_RetainObject ( ZLRefCountedObject* object ) {
	UNUSED ( object );
}

//----------------------------------------------------------------//
void MOAIDraw::MOAIAbstractDrawingAPI_SubmitCommand ( MOAIDrawingAPIEnum::_ cmd, const void* param, ZLSize size ) {

	MOAIDrawingAPI::Execute ( 0, cmd, param );
}
