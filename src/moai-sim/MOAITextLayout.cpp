// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/moai_utf8.h>
#include <moai-sim/MOAIAnimCurve.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAINodeMgr.h>
#include <moai-sim/MOAIQuadBrush.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAITextDesigner.h>
#include <moai-sim/MOAITextLayout.h>
#include <moai-sim/MOAITextStyle.h>
#include <moai-sim/MOAITextStyleParser.h>

#define DEFAULT_STYLE_NAME ""

//================================================================//
// MOAITextLayout
//================================================================//

//----------------------------------------------------------------//
void MOAITextLayout::AddHighlight ( u32 base, u32 top, u32 color ) {

	// make room for the new highlight
	this->ClearHighlight ( base, top );

	MOAITextHighlight* highlight = new MOAITextHighlight ();
	
	highlight->mBase	= base;
	highlight->mTop		= top;
	highlight->mColor	= color;
	highlight->mPrev	= 0;
	highlight->mNext	= 0;
	
	if ( !this->mHighlights ) {
		this->mHighlights = highlight;
		return;
	}
	
	MOAITextHighlight* best = 0;
	MOAITextHighlight* cursor = this->mHighlights;
	for ( ; cursor; cursor = cursor->mNext ) {
		if ( cursor->mBase >= base ) break;
		best = cursor;
	}
	
	if ( best ) {
	
		highlight->mNext = best->mNext;
		highlight->mPrev = best;
	
		if ( best->mNext ) {
			best->mNext->mPrev = highlight;
		}
		best->mNext = highlight;
	}
	else {
	
		highlight->mNext = this->mHighlights;
		this->mHighlights = highlight;
		
		if ( highlight->mNext ) {
			highlight->mNext->mPrev = highlight;
		}
	}
}

//----------------------------------------------------------------//
void MOAITextLayout::ApplyHighlights () {

	u32 totalSprites = this->mSprites.GetTop ();
	u32 spriteIdx = 0;
	MOAITextHighlight* highlight = this->mHighlights;
	
	while (( spriteIdx < totalSprites ) && highlight ) {
		MOAITextSprite& sprite = this->mSprites [ spriteIdx ];
		
		if ( sprite.mIdx >= highlight->mTop ) {
			highlight = highlight->mNext;
		}
		else {
		
			if ( sprite.mIdx >= highlight->mBase ) {
				sprite.mRGBA = highlight->mColor;
				sprite.mMask |= MOAITextSprite::MASK_COLOR;
			}
			spriteIdx++;
		}
	}
}

//----------------------------------------------------------------//
void MOAITextLayout::ClearHighlight ( u32 base, u32 top ) {

	MOAITextHighlight* highlight = this->mHighlights;
	for ( ; highlight; highlight = highlight->mNext ) {
		
		if ( highlight->mTop <= base ) continue;
		if ( highlight->mBase >= top ) continue;
		
		// cursor is inside of span
		if (( highlight->mBase >= base ) && ( highlight->mTop <= top )) {
			highlight->mBase = highlight->mTop;
			continue;
		}
		
		// cursor contains span
		if (( highlight->mBase < base ) && ( highlight->mTop > top )) {
			
			MOAITextHighlight* split = new MOAITextHighlight ();
			
			split->mColor	= highlight->mColor;
			split->mBase	= top;
			split->mTop		= highlight->mTop;
			
			split->mPrev = highlight;
			split->mNext = highlight->mNext;
			
			highlight->mTop = base;
			
			if ( highlight->mNext ) {
				highlight->mNext->mPrev = split;
			}
			highlight->mNext = split;
			break;
		}
		
		if (( highlight->mBase < base ) && ( base < highlight->mTop )) {
			highlight->mTop = base;
			continue;
		}
		
		if (( highlight->mBase < top ) && ( top < highlight->mTop )) {
			highlight->mBase = top;
			continue;
		}
	}
}

//----------------------------------------------------------------//
void MOAITextLayout::ClearHighlights () {

	MOAITextHighlight* cursor = this->mHighlights;
	this->mHighlights = 0;
	
	while ( cursor ) {
		MOAITextHighlight* highlight = cursor;
		cursor = cursor->mNext;
		delete highlight;
	}
}

//----------------------------------------------------------------//
void MOAITextLayout::CompactHighlights () {

	MOAITextHighlight* cursor = this->mHighlights;
	while ( cursor ) {
	
		MOAITextHighlight* highlight = cursor;
		cursor = highlight->mNext;
	
		// prune empty highlights
		if ( highlight->mBase == highlight->mTop ) {
			this->RemoveHighlight ( *highlight );
			delete highlight;
			continue;
		}
		
		// gobble up any adjacent (or overlapping) same-colored highlights
		// break if we hit a gap or another color
		MOAITextHighlight* cursor2 = highlight->mNext;
		while ( cursor2 ) {
			MOAITextHighlight* highlight2 = cursor2;
			cursor2 = highlight2->mNext;
			
			if ( highlight2->mBase != highlight2->mTop ) {
			
				if ( highlight->mColor != highlight2->mColor ) break;
				if ( highlight->mTop < highlight2->mBase ) break;
				
				if ( highlight->mTop < highlight2->mTop ) {
					highlight->mTop = highlight2->mTop;
				}
			}
			
			this->RemoveHighlight ( *highlight2 );
			delete highlight2;
		}
		
		cursor = highlight->mNext;
	}
}

//----------------------------------------------------------------//
u32 MOAITextLayout::CountSprites () {

	return this->mSprites.GetTop ();
}

//----------------------------------------------------------------//
void MOAITextLayout::Draw ( u32 reveal ) {
	
	if ( reveal ) {
		
		MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
		MOAIQuadBrush::BindVertexFormat ( gfxDevice );

		ZLColorVec baseColor = gfxDevice.GetPenColor ();
		ZLColorVec blendColor;
		u32 rgba0 = 0xffffffff;
		u32 rgba1 = 0xffffffff;

		u32 size = this->mSprites.GetTop ();
		for ( u32 i = 0; ( i < size ) && ( i < reveal ); ++i ) {
			const MOAITextSprite& sprite = this->mSprites [ i ];
			const MOAITextStyle* style = sprite.mStyle;
			
			rgba0 = ( sprite.mMask & MOAITextSprite::MASK_COLOR ) ? sprite.mRGBA : sprite.mStyle->mColor;
			
			if ( rgba0 != rgba1 ) {
				rgba1 = rgba0;
				
				blendColor.SetRGBA ( rgba0 );
				blendColor.Modulate ( baseColor );
				gfxDevice.SetPenColor ( blendColor );
			}
			sprite.mGlyph->Draw ( *sprite.mTexture, sprite.mPen.mX, sprite.mPen.mY, sprite.mScale.mX, sprite.mScale.mY, style->mPadding );
		}
	}
}

//----------------------------------------------------------------//
void MOAITextLayout::DrawDebug () {

	MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
	
	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008
	
	if ( debugLines.Bind ( MOAIDebugLines::TEXT_BOX_GLYPHS )) {
	
		u32 size = this->mSprites.GetTop ();
		for ( u32 i = 0; i < size; ++i ) {
			const MOAITextSprite& sprite = this->mSprites [ i ];
			
			ZLRect glyphRect = sprite.mGlyph->GetRect ( sprite.mPen.mX, sprite.mPen.mY, sprite.mScale.mX, sprite.mScale.mY );
			
			const ZLRect& padding = sprite.mStyle->mPadding;
			glyphRect.mXMin += padding.mXMin;
			glyphRect.mYMin += padding.mYMin;
			glyphRect.mXMax += padding.mXMax;
			glyphRect.mYMax += padding.mYMax;
			
			draw.DrawRectOutline ( glyphRect );
		}
	}
	
	if ( debugLines.Bind ( MOAIDebugLines::TEXT_BOX_LAYOUT )) {
		
		u32 totalLines = this->mLines.GetTop ();
		for ( u32 i = 0; i < totalLines; ++i ) {
			MOAITextLine& line = this->mLines [ i ];
			draw.DrawRectOutline ( line.mRect );
		}
	}
	
	if ( debugLines.Bind ( MOAIDebugLines::TEXT_BOX_BASELINES )) {
		
		u32 totalLines = this->mLines.GetTop ();
		for ( u32 i = 0; i < totalLines; ++i ) {
			MOAITextLine& line = this->mLines [ i ];
			float y = line.mRect.mYMin + line.mAscent;
			draw.DrawLine ( line.mRect.mXMin, y, line.mRect.mXMax, y );
		}
	}
}

//----------------------------------------------------------------//
void MOAITextLayout::FindSpriteSpan ( u32 idx, u32 size, u32& spanIdx, u32& spanSize ) {

	spanSize = 0;

	u32 top = this->mSprites.GetTop ();
	if ( !top ) return;
	if ( this->mSprites [ 0 ].mIdx >= ( idx + size )) return;
	if ( this->mSprites [ top - 1 ].mIdx < idx ) return;
	
	for ( u32 i = 0; i < top; ++i ) {
		MOAITextSprite& sprite = this->mSprites [ i ];
	
		if ( sprite.mIdx >= idx ) {
			spanIdx = i;
			spanSize = 1;
			break;
		}
	}
	
	if ( spanSize ) {
		
		u32 max = idx + size;
		u32 i = spanIdx + 1;
		u32 count = 0;
		
		for ( ; i < top; ++i, ++count ) {
			MOAITextSprite& sprite = this->mSprites [ i ];
			if ( sprite.mIdx >= max ) break;
		}
		spanSize = count + 1;
	}
}

//----------------------------------------------------------------//
bool MOAITextLayout::GetBounds ( ZLRect& rect ) {

	if ( this->mSprites.GetTop () > 0 ) {
		rect = this->mBounds;
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAITextLayout::GetBoundsForRange ( u32 idx, u32 size, ZLRect& rect ) {

	if ( !size ) return false;

	bool result = false;

	u32 spanIdx;
	u32 spanSize;
	
	this->FindSpriteSpan ( idx, size, spanIdx, spanSize );
	if ( !spanSize ) return false;

	u32 end = spanIdx + spanSize;
	for ( u32 i = spanIdx; i < end; ++i ) {
		MOAITextSprite& sprite = this->mSprites [ i ];
		MOAIGlyph& glyph = *sprite.mGlyph;

		if ( glyph.mWidth > 0.0f ) {

			ZLRect glyphRect = glyph.GetRect ( sprite.mPen.mX, sprite.mPen.mY, sprite.mScale.mX, sprite.mScale.mY );

			// Update the glyphRect height with the size of the of the glyphset's height for
			// the max possible line height.
			float fontSize = sprite.mStyle->GetSize ();
			MOAIGlyphSet* glyphSet = sprite.mStyle->GetFont ()->GetGlyphSet ( fontSize );
			float deckHeight = glyphSet->GetHeight () * sprite.mScale.mY;
			float deckAscent = glyphSet->GetAscent () * sprite.mScale.mY;
			glyphRect.mYMax = sprite.mPen.mY - deckAscent;
			glyphRect.mYMin = glyphRect.mYMax - deckHeight;

			if ( result ) {
				rect.Grow ( glyphRect );
			}
			else {
				rect = glyphRect;
				result = true;
			}
		}
	}
	return result;
}

//----------------------------------------------------------------//
MOAITextLayout::MOAITextLayout () :
	mXOffset ( 0.0f ),
	mYOffset ( 0.0f ),
	mHighlights ( 0 ) {
}

//----------------------------------------------------------------//
MOAITextLayout::~MOAITextLayout () {

	this->Reset ();
}

//----------------------------------------------------------------//
void MOAITextLayout::PushLine ( u32 start, u32 size, const ZLRect& rect, float height, float ascent ) {

	MOAITextLine textLine;
	
	textLine.mStart			= start;
	textLine.mSize			= size;
	textLine.mRect			= rect;
	textLine.mHeight		= height;
	textLine.mAscent		= ascent;
	
	this->mLines.Push ( textLine );
}

//----------------------------------------------------------------//
void MOAITextLayout::PushSprite ( u32 idx, MOAIGlyph& glyph, MOAITextStyle& style, float x, float y, float xScale, float yScale ) {
	
	MOAITextSprite textSprite;
	
	textSprite.mIdx			= idx;
	textSprite.mGlyph		= &glyph;
	textSprite.mStyle		= &style;
	textSprite.mPen.mX		= x;
	textSprite.mPen.mY		= y;
	textSprite.mScale.mX	= xScale;
	textSprite.mScale.mY	= yScale;
	
	textSprite.mRGBA		= style.mColor;
	textSprite.mTexture		= style.mFont->GetGlyphTexture ( glyph );
	textSprite.mMask		= 0;

	this->mSprites.Push ( textSprite );
}

//----------------------------------------------------------------//
void MOAITextLayout::RemoveHighlight ( MOAITextHighlight& highlight ) {

	MOAITextHighlight* prev = highlight.mPrev;
	MOAITextHighlight* next = highlight.mNext;
	
	if ( prev ) {
		prev->mNext = next;
	}
	else {
		this->mHighlights = next;	
	}
	
	if ( next ) {
		next->mPrev = prev;
	}
}

//----------------------------------------------------------------//
void MOAITextLayout::Reset () {

	this->mLines.Reset ();
	this->mSprites.Reset ();
}

//----------------------------------------------------------------//
void MOAITextLayout::ResetHighlights () {

	u32 top = this->mSprites.GetTop ();
	for ( u32 i = 0; i > top; ++i ) {
		MOAITextSprite& sprite = this->mSprites [ i ];
		sprite.mMask = sprite.mMask & ~MOAITextSprite::MASK_COLOR;
	}
}

//----------------------------------------------------------------//
void MOAITextLayout::SetHighlight ( u32 idx, u32 size ) {

	if ( !size ) return;

	this->ClearHighlight ( idx, idx + size );
	this->CompactHighlights ();
	this->ResetHighlights ();
	this->ApplyHighlights ();
}

//----------------------------------------------------------------//
void MOAITextLayout::SetHighlight ( u32 idx, u32 size, u32 color ) {

	if ( !size ) return;

	this->AddHighlight ( idx, idx + size, color );
	this->CompactHighlights ();
	this->ResetHighlights ();
	this->ApplyHighlights ();
}
