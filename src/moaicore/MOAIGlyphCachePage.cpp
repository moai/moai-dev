// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIFont.h>
#include <moaicore/MOAIGlyph.h>
#include <moaicore/MOAIGlyphCachePage.h>
#include <moaicore/MOAIImageTexture.h>

#define MAX_TEXTURE_SIZE 1024

//================================================================//
// MOAIGlyphCachePage
//================================================================//

//----------------------------------------------------------------//
void MOAIGlyphCachePage::AffirmCanvas ( MOAIFont& font ) {
	
	if ( !this->mImageTexture ) {
		
		this->mImageTexture = new MOAIImageTexture ();
		this->mImageTexture->Init ( MAX_TEXTURE_SIZE, this->mRows.mSize, this->mColorFormat, USPixel::TRUECOLOR );
		this->mImageTexture->SetDebugName ( font.GetFilename ());
		this->mImageTexture->SetFilter ( GL_LINEAR, GL_LINEAR );
		this->mImageTexture->ClearBitmap ();
	}
	else if ( this->mImageTexture->MOAIImage::GetHeight () < this->mRows.mSize ) {
		
		USIntRect rect;
		rect.Init ( 0, 0, MAX_TEXTURE_SIZE, this->mRows.mSize );
		this->mImageTexture->ResizeCanvas ( *this->mImageTexture, rect );
		this->mImageTexture->Invalidate ();
	}
}

//----------------------------------------------------------------//
MOAIGlyphCachePage::GlyphSpan* MOAIGlyphCachePage::Alloc ( MOAIFont& font, MOAIGlyph& glyph ) {
	
	u32 width = ( u32 )glyph.mWidth + 2;
	u32 height = ( u32 )glyph.mHeight + 2;
	
	RowSpan* rowIt = this->mRows.mHead;
	RowSpan* bestRowIt = 0;
	RowSpan* backupRowIt = 0;
	
	// find the very shortest row that can still accomodate the glyph
	for ( ; rowIt; rowIt = rowIt->mNext ) {
		if ( rowIt->mOccupied && ( height <= rowIt->mSize ) && rowIt->mData.HasRoom ( width )){
			if ( !bestRowIt || ( bestRowIt && ( rowIt->mSize < bestRowIt->mSize ))) {
				bestRowIt = rowIt;
			}
		}
	}
	
	// save a ref to the best row in case we need to fall back on it
	backupRowIt = bestRowIt;
	
	// if the best row is still too tall for us, throw it away
	// 'too tall' means that the row is going to be too loose a fit as
	// given by mThreshold
	if ( bestRowIt && (( u32 )( bestRowIt->mSize * this->mThreshold ) > height )) {
		bestRowIt = 0;
	}
	
	// if no suitable match was found or the best row wasn't best enough, try to alloc a new row
	if ( !bestRowIt ) {
		
		bestRowIt = this->AllocRow ( height );
		
		// alloc failed and no fall back available
		// only choice is to try and expand...
		if ( !bestRowIt ) {
			
			// expand page to next power of two and try again
			// keep trying until the alloc succeeds or we max out
			while ( !bestRowIt && this->ExpandToNextPowerofTwo ()) {
				bestRowIt = this->AllocRow ( height );
			}
			
			// if expand/alloc failed, fall back on original match
			if ( !bestRowIt ) {
				bestRowIt = backupRowIt;
			}
			
			// if we're still failing, give up
			if ( !bestRowIt ) return 0;
		}
	}
	
	GlyphSpan* glyphSpan = bestRowIt->mData.Alloc ( width );
	if ( glyphSpan ) {
		glyph.SetSourceLoc ( glyphSpan->mBase, bestRowIt->mBase );
	}
	
	this->AffirmCanvas ( font );
	return glyphSpan;
}

//----------------------------------------------------------------//
MOAIGlyphCachePage::RowSpan* MOAIGlyphCachePage::AllocRow ( u32 height ) {

	RowSpan* rowIt = this->mRows.Alloc ( height );
		
	// if alloc succeeded, initialize the new row
	if ( rowIt ) {
		//u32 maxTextureSize = MOAIGfxDevice::Get ().GetMaxTextureSize ();
		u32 maxTextureSize = MAX_TEXTURE_SIZE;
		rowIt->mData.Expand ( maxTextureSize );
	}
	return rowIt;
}

//----------------------------------------------------------------//
void MOAIGlyphCachePage::Clear () {

	if ( this->mImageTexture ) {
		delete this->mImageTexture;
		this->mImageTexture = 0;
	}
}

//----------------------------------------------------------------//
bool MOAIGlyphCachePage::ExpandToNextPowerofTwo () {

	//u32 maxTextureSize = MOAIGfxDevice::Get ().GetMaxTextureSize ();
	u32 maxTextureSize = MAX_TEXTURE_SIZE;
	if ( this->mRows.mSize >= maxTextureSize ) return false;
	
	size_t size = this->mRows.mSize ? this->mRows.mSize << 1 : 8;
	this->mRows.Expand ( size );
	
	return true;
}

//----------------------------------------------------------------//
void MOAIGlyphCachePage::InitCanvas ( u32 width, u32 height, USColor::Format colorFmt, USPixel::Format pixelFmt ) {

	this->Clear ();

	this->mImageTexture = new MOAIImageTexture ();
	this->mImageTexture->Init ( width, height, colorFmt, pixelFmt );
}

//----------------------------------------------------------------//
MOAIGlyphCachePage::MOAIGlyphCachePage () :
	mImageTexture ( 0 ),
	mColorFormat ( USColor::A_8 ),
	mThreshold ( 0.8f ) {
}

//----------------------------------------------------------------//
MOAIGlyphCachePage::~MOAIGlyphCachePage () {

	this->Clear ();
}
