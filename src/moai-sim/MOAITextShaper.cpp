// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGlyph.h>
#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAITextLayoutRules.h>
#include <moai-sim/MOAITextShaper.h>

//================================================================//
// MOAITextSimpleShaper
//================================================================//

//----------------------------------------------------------------//
MOAITextSimpleShaper::MOAITextSimpleShaper () {
}

//----------------------------------------------------------------//
MOAITextSimpleShaper::~MOAITextSimpleShaper () {
}

//----------------------------------------------------------------//
u32 MOAITextSimpleShaper::ShapeLine ( MOAITextShaperClient& client, const MOAITextLayoutRules& layoutRules ) {

	// return one of overrun modes (in an overrun occurs)
	u32 wrap = MOAITextLayoutRules::OVERRUN_NONE;

	float penX				= 0.0f;
	MOAIGlyph* prevGlyph	= 0;
	u32 tokenSize			= 0;
	u32 tokenCount			= 0;

	// some compilers gripe if a constant is used as a loop conditional. those compilers are pussies.
	bool more = true;
	while ( more ) {
	
		// this sets restore point to start of the current character; also prepares for the next call to BeginToken ()
		client.BeginChar ();
	
		MOAITextStyledChar styledChar = client.NextChar ();
		u32 c = styledChar.mChar;
		
		if ( MOAIFont::IsControl ( c )) break; // only /n is supported. TODO: properly support /t and /r
		
		MOAIGlyph* glyph = styledChar.mGlyph;
		if ( !glyph ) continue; // TODO: this just omits missing glyphs. think about rendering a solid color instead.
		
		u32 code = glyph->GetCode ();
		
		if ( code == MOAIGlyph::NULL_CODE_ID ) continue; // should also handle this more better
		
		float xScale = styledChar.mScale.mX;
		
		// apply kerning
		if ( prevGlyph ) {
		
			MOAIKernVec kernVec = prevGlyph->GetKerning ( code );
			penX += kernVec.mX * xScale;
		}
		
		prevGlyph = glyph;
		
		if ( MOAIFont::IsWhitespace ( c )) {
			
			tokenSize = 0; // accept the token
			tokenCount++;
		}
		else {
			
			// handle new token
			if ( tokenSize == 0 ) {
			
				// sets the resume point for WRAP_TOKEN; just a copy of the
				// resume point set by BeginChar ()
				client.BeginToken ();
			
				if ( wrap == MOAITextLayoutRules::OVERRUN_TRUNCATE_WORD ) {
					break;
				}
			}
			tokenSize++;
			
			if ( wrap == MOAITextLayoutRules::OVERRUN_NONE ) {
			
				u32 result = client.PushSprite ( styledChar, penX, 0.0f );
				
				if ( result == MOAITextShaperClient::PUSH_OVERRUN ) {
					
					// there was an overrun, so we're either going to discard the last char or the last token
					wrap = ( tokenCount == 0 ) ? layoutRules.GetFirstOverrunRule () : layoutRules.GetOverrunRule ();
					
					if (( wrap != MOAITextLayoutRules::OVERRUN_NONE ) && ( wrap != MOAITextLayoutRules::OVERRUN_TRUNCATE_WORD )) break;
				}
			}
		}
		
		// advance the pen
		penX += glyph->mAdvanceX * xScale;
	}
	
	return wrap;
}
