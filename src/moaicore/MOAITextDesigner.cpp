// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/utf8.h>
#include <moaicore/MOAIFont.h>
#include <moaicore/MOAITextLayout.h>
#include <moaicore/MOAITextDesigner.h>
#include <moaicore/MOAITextStyle.h>
#include <moaicore/MOAITextStyleMap.h>

//================================================================//
// MOAITextDesigner
//================================================================//

//----------------------------------------------------------------//
//void MOAITextDesigner::FlushLine () {
//
//	if ( this->mLineCount >= this->mTotalLines ) return;
//
//	float x = this->mFrame.mXMin;
//	float width = this->mFrame.Width ();
//	
//	float lineWidth = this->mRightToLeft ? -this->mLineXMax : this->mLineXMax;
//	float lineHeight = this->mFont->GetLineSpacing () * this->mPoints * this->mLineSpacing;
//
//	switch ( this->mJustify ) {
//		
//		case CENTER_JUSTIFY:
//			x = ( x + ( width * 0.5f )) - ( lineWidth * 0.5f );
//			
//		case LEFT_JUSTIFY:
//			break;
//
//		case RIGHT_JUSTIFY:
//			x = this->mFrame.mXMax - lineWidth;
//	}
//	
//	MOAIAnimCurve* curve = 0;
//	if ( this->mCurves ) {
//		curve = this->mCurves [ this->mLineCount % this->mTotalCurves ];
//	}
//	
//	if ( this->mRightToLeft ) {
//		x -= this->mLineXMax;
//	}
//	
//	for ( u32 i = this->mLineBottom; i < this->mLineTop; ++i ) {
//		
//		MOAITextSprite& sprite = ( *this->mLayout )[ i ];
//		
//		float yOff = 0.0f;
//		if ( curve ) {
//			yOff = curve->GetFloatValue ( sprite.mX / width );
//		}
//
//		sprite.mX += x;
//		sprite.mY = this->mPen.mY + yOff;
//	}
//	
//	this->mLineBottom = this->mLineTop;
//	this->mLineXMax = 0.0f;
//	this->mPen.mY += lineHeight;
//	this->mLineCount++;
//	this->mGlyph = 0;
//	
//	// move token to origin
//	u32 tokenTop = this->mLayout->GetTop ();
//	for ( u32 i = this->mLineTop; i < tokenTop; ++i ) {
//		
//		MOAITextSprite& sprite = ( *this->mLayout )[ i ];
//		sprite.mX -= this->mTokenXMin;
//	}
//
//	this->mPen.mX -= this->mTokenXMin;
//	this->mTokenXMax -= this->mTokenXMin;
//	this->mTokenXMin = 0.0f;
//}

//----------------------------------------------------------------//
//void MOAITextDesigner::FlushToken () {
//	
//	float width = this->mFrame.Width ();
//	float tokenMax = this->mRightToLeft ? -this->mTokenXMax : this->mTokenXMax;
//	
//	if (( this->mLineXMax != 0.0f ) && ( tokenMax >= width )) {
//		this->FlushLine ();
//	}
//	
//	if ( this->mLineCount < this->mTotalLines ) {
//		this->mLineTop = this->mLayout->GetTop ();
//		this->mLineXMax = this->mTokenXMax;
//	}
//}

//----------------------------------------------------------------//
bool MOAITextDesigner::IsWhitespace ( u32 c ) {

	if ( !c ) return true;
	if ( c == ' ' ) return true;
	if ( c == '\t' ) return true;
	if ( c == '\n' ) return true;
	
	return false;
}

//----------------------------------------------------------------//
void MOAITextDesigner::Layout ( cc8* str, MOAITextStyleMap& styleMap, MOAITextLayout& layout ) {
	
	this->mStr = str;
	
	this->mSpanIdx = 0;
	this->mStyleMap = &styleMap;
	this->mStyleSpan = &this->mStyleMap->Elem ( 0 );
	this->mIdx = this->mStyleSpan->mBase;
	
	this->UpdateStyle ();
	
	//float xScaleAdvance = this->mRightToLeft ? -1.0f : 1.0f;
	float xScaleAdvance = 1.0f;

	USVec2D pen;
	pen.Init ( 0.0f, 0.0f );
	
	float scale = 1.0f;
	
	//bool inWhitespace = false;
	//bool gobbleWhitespace = true;
	const MOAIGlyph* glyph = 0;
	const MOAIGlyph* prevGlyph = 0;
	
	//u32 spanIdx = 0;
	//u32 idx = 0;
	
	bool more = true;
	while ( more ) {
	
		u32 c = this->NextChar ();
		
		//inWhitespace = this->IsWhitespace ( c );
		
		if ( c == 0 ) break;
		putc ( c, stdout );
		
		prevGlyph = glyph;
		glyph = this->mDeck->GetGlyph ( c );
		if ( !glyph ) continue;
		
		// apply kerning
		if ( prevGlyph ) {
			MOAIKernVec kernVec = prevGlyph->GetKerning ( glyph->mCode );
			pen.mX += kernVec.mX * scale * xScaleAdvance;
		}
		
		// push the glyph
		//float penX = pen.mX + (( glyph->mWidth + glyph->mBearingX ) * scale * xScaleAdvance );
		float glyphX = pen.mX;
		//float glyphX = this->mRightToLeft ? penX : this->mPen.mX;

		layout.PushGlyph ( glyph, 0, glyphX, 0.0f, 0xffffffff );
		//this->mTokenXMax = penX;

		pen.mX += glyph->mAdvanceX * scale * xScaleAdvance;
		
		//this->WriteChar ( c );
	}
	
	printf ( "\n" );
}

//----------------------------------------------------------------//
MOAITextDesigner::MOAITextDesigner () {
}

//----------------------------------------------------------------//
MOAITextDesigner::~MOAITextDesigner () {
}

//----------------------------------------------------------------//
u32 MOAITextDesigner::NextChar () {

	if ( !this->mStyleSpan ) return 0;

	u32 c = u8_nextchar ( this->mStr, &this->mIdx );

	if ( this->mIdx >= this->mStyleSpan->mTop ) {
		
		this->mSpanIdx++;
		this->mStyleSpan = 0;
		
		if ( this->mSpanIdx < this->mStyleMap->GetTop ()) {
			this->mStyleSpan = &this->mStyleMap->Elem ( this->mSpanIdx );
			this->mIdx = this->mStyleSpan->mBase;
			this->UpdateStyle ();
		}
	}
	
	return c;
}

//----------------------------------------------------------------//
void MOAITextDesigner::UpdateStyle () {

	assert ( this->mStyleSpan );
	
	MOAITextStyle* style = this->mStyleSpan->mStyle;
	assert ( style );
	
	MOAIFont* font = style->mFont;
	assert ( this->mStyleSpan->mStyle->mFont );
	
	this->mDeck = font->GetGlyphDeck ( style->mPoints );
}