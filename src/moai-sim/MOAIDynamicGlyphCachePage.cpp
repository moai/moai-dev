// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAIDynamicGlyphCache.h>
#include <moai-sim/MOAIDynamicGlyphCachePage.h>
#include <moai-sim/MOAIGlyph.h>

#define MAX_TEXTURE_SIZE 1024

//================================================================//
// MOAIDynamicGlyphCachePage
//================================================================//

//----------------------------------------------------------------//
void MOAIDynamicGlyphCachePage::AffirmCanvas ( MOAIDynamicGlyphCache& owner, MOAIFont& font ) {
	
	if ( !this->mImageTexture ) {
		
		STLString debugName;
		debugName.write ( "page %d - %s (%p)", ( int )this->mPageID, font.GetFilename (), &font );
		
		this->mImageTexture = owner.Get < MOAIGfxMgr >().CreateImageTexture ();
		this->mImageTexture->Init ( MAX_TEXTURE_SIZE, ( u32 )this->mRows.mSize, owner.mColorFormat, MOAIImage::TRUECOLOR );
		this->mImageTexture->SetDebugName ( debugName );
		this->mImageTexture->SetFilter ( font.GetMinFilter (), font.GetMagFilter ());
		this->mImageTexture->ClearBitmap ();
		
		owner.LuaRetain ( this->mImageTexture );
	}
	else if ( this->mImageTexture->MOAIImage::GetHeight () < this->mRows.mSize ) {
		
		ZLIntRect rect ( 0, 0, MAX_TEXTURE_SIZE, ( int )this->mRows.mSize );
		this->mImageTexture->ResizeCanvas ( *this->mImageTexture, rect );
		this->mImageTexture->UpdateRegion ();
	}
}

//----------------------------------------------------------------//
MOAIDynamicGlyphCachePage::GlyphSpan* MOAIDynamicGlyphCachePage::Alloc ( MOAIDynamicGlyphCache& owner, MOAIFont& font, MOAIGlyph& glyph ) {
	
	u32 width = ( u32 )( glyph.mWidth + owner.mPadding.Width ());
	u32 height = ( u32 )( glyph.mHeight + owner.mPadding.Height ());
	
	RowSpan* rowIt = this->mRows.mHead;
	RowSpan* bestRowIt = 0;
	RowSpan* backupRowIt = 0;
	
	// find the very shortest row that can still accommodate the glyph
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
		
		// alloc failed and no fallback available
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
		glyph.SetSourceLoc (( u32 )( glyphSpan->mBase - ( s32 )owner.mPadding.mXMin ), ( u32 )( bestRowIt->mBase - ( s32 ) owner.mPadding.mYMin ));
	}
	
	this->AffirmCanvas ( owner, font );
	return glyphSpan;
}

//----------------------------------------------------------------//
MOAIDynamicGlyphCachePage::RowSpan* MOAIDynamicGlyphCachePage::AllocRow ( u32 height ) {

	RowSpan* rowIt = this->mRows.Alloc ( height );
		
	// if alloc succeeded, initialize the new row
	if ( rowIt ) {
		//u32 maxTextureSize = MOAIGfxMgr::Get ().GetMaxTextureSize ();
		u32 maxTextureSize = MAX_TEXTURE_SIZE;
		rowIt->mData.Expand ( maxTextureSize );
	}
	return rowIt;
}

//----------------------------------------------------------------//
void MOAIDynamicGlyphCachePage::Clear ( MOAIDynamicGlyphCache& owner ) {

	if ( this->mImageTexture ) {
		owner.LuaRelease ( this->mImageTexture );
		//delete this->mImageTexture;
		this->mImageTexture = 0;
	}
}

//----------------------------------------------------------------//
bool MOAIDynamicGlyphCachePage::ExpandToNextPowerofTwo () {

	//u32 maxTextureSize = MOAIGfxMgr::Get ().GetMaxTextureSize ();
	u32 maxTextureSize = MAX_TEXTURE_SIZE;
	if ( this->mRows.mSize >= maxTextureSize ) return false;
	
	size_t size = this->mRows.mSize ? this->mRows.mSize << 1 : 8;
	this->mRows.Expand ( size );
	
	return true;
}

//----------------------------------------------------------------//
MOAIDynamicGlyphCachePage::MOAIDynamicGlyphCachePage () :
	mPageID ( 0 ),
	mImageTexture ( 0 ),
	//mColorFormat ( ZLColor::A_8 ),
	mThreshold ( 0.8f ) {
}

//----------------------------------------------------------------//
MOAIDynamicGlyphCachePage::~MOAIDynamicGlyphCachePage () {

	assert ( !this->mImageTexture ); // call Clear () w/ owner
}
