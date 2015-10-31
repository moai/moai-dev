// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGlyph.h>
#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAITextDesigner.h>
#include <moai-sim/MOAITextShaper.h>

//================================================================//
// MOAITextSimpleShaper
//================================================================//

//----------------------------------------------------------------//
void MOAITextSimpleShaper::AcceptToken () {

//	this->mTokenSize = 0;
//	this->mTokenIdx = this->mPrevIdx;
//	this->mTokenSpriteID = this->mLayout->mSprites.GetTop ();
}

//----------------------------------------------------------------//
MOAITextSimpleShaper::MOAITextSimpleShaper () {
}

//----------------------------------------------------------------//
MOAITextSimpleShaper::~MOAITextSimpleShaper () {
}

//----------------------------------------------------------------//
void MOAITextSimpleShaper::ShapeLine ( MOAITextStyledCharStream& charStream, MOAITextLineLayout& lineLayout, const MOAITextDesigner& designer ) {

	u32 lineSize			= 0;
	float penX				= 0.0f;
	MOAIGlyph* prevGlyph	= 0;
	u32 tokenSize			= 0;
	u32 tokenCharIdx		= charStream.GetCharIdx ();
	u32 tokenSpriteIdx		= lineLayout.GetSpriteIdx ();

	float frameWidth		= designer.GetFrame ().Width ();
	bool limitWidth			= designer.GetLimitWidth ();

	bool more = true;
	while ( more ) {
	
		MOAITextStyledChar styledChar = charStream.NextChar ();
		u32 c = styledChar.mChar;
		
		if ( MOAIFont::IsControl ( c )) break;
		
		MOAIGlyph* glyph = styledChar.mGlyph;
		if ( !glyph ) continue;
		
		u32 code = glyph->GetCode ();
		
		if ( code == MOAIGlyph::NULL_CODE_ID ) continue;
		
		float xScale = styledChar.mScale.mX;
		
		// apply kerning
		if ( prevGlyph ) {
			MOAIKernVec kernVec = prevGlyph->GetKerning ( code );
			penX += kernVec.mX * xScale;
		}
		
		prevGlyph = glyph;
		
		if ( MOAIFont::IsWhitespace ( c )) {
			
			tokenSize = 0; // accept the token
		}
		else {
			
			// handle new token
			if ( tokenSize == 0 ) {
				tokenCharIdx = charStream.GetCharIdx ();
				tokenSpriteIdx = lineLayout.GetSpriteIdx ();
			}
			
			lineLayout.PushSprite ( styledChar, penX, 0.0f );
			tokenSize++;
		}
		
		// advance the pen
		penX += glyph->mAdvanceX * xScale;
	}
	
	// discard the token
//	if ( tokenSize > 0 ) {
//		charStream.SeekChar ( tokenCharIdx );
//		lineLayout.SeekSprite ( tokenSpriteIdx );
//	}
}
