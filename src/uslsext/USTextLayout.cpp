// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USBinarySearch.h>
#include <uslsext/USDrawBuffer.h>
#include <uslsext/USFont.h>
#include <uslsext/USGLQuad.h>
#include <uslsext/USTextLayout.h>

//================================================================//
// USTextLayout
//================================================================//

//----------------------------------------------------------------//
void USTextLayout::Draw ( u32 reveal ) {

	USDrawBuffer& drawBuffer = USDrawBuffer::Get ();
	USGLQuad::BindVertexFormat ( drawBuffer );

	USColorVec baseColor = drawBuffer.GetPenColor ();
	USColorVec blendColor;
	u32 rgba = 0xffffffff;

	u32 size = this->GetTop ();
	for ( u32 i = 0; ( i < size ) && ( i < reveal ); ++i ) {
		const USTextSprite& sprite = ( *this )[ i ];
		
		if ( sprite.mRGBA != rgba ) {
			rgba = sprite.mRGBA;
			
			blendColor.SetRGBA ( rgba );
			blendColor.Modulate ( baseColor );
			drawBuffer.SetPenColor ( blendColor );
		}
		sprite.mGlyph->Draw ( sprite.mPoints, sprite.mX, sprite.mY );
	}
}

//----------------------------------------------------------------//
bool USTextLayout::GetBoundsForRange ( u32 idx, u32 size, USRect& rect ) {

	bool result = false;
	u32 end = idx + size;
	float x = 0.0f;

	// TODO: replace w/ binary search
	u32 top = this->GetTop ();
	for ( u32 i = 0; i < top; ++i ) {
		USTextSprite& sprite = ( *this )[ i ];
		
		if ( sprite.mIdx >= end ) break;

		if ( sprite.mIdx >= idx ) {
			
			const USGlyph* glyph = sprite.mGlyph;
			
			if ( glyph->mWidth ) {
				
				USRect glyphRect = glyph->GetRect ( sprite.mPoints, sprite.mX, sprite.mY );
				
				if ( result ) {
					if ( sprite.mX < x ) break;
					rect.Grow ( glyphRect );
				}
				else {
					rect = glyphRect;
					result = true;
				}
				x = sprite.mX;
			}
		}
	}
	return result;
}

//----------------------------------------------------------------//
void USTextLayout::PushGlyph ( const USGlyph* glyph, u32 idx, float x, float y, float points, u32 rgba ) {

	USTextSprite textSprite;
	
	textSprite.mGlyph		= glyph;
	textSprite.mIdx			= idx;
	textSprite.mX			= x;
	textSprite.mY			= y;
	textSprite.mRGBA		= rgba;
	textSprite.mPoints		= points;

	this->Push ( textSprite );
}
