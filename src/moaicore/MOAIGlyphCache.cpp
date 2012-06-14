// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/utf8.h>
#include <moaicore/MOAIDataBuffer.h>
#include <moaicore/MOAIFont.h>
#include <moaicore/MOAIGlyphCache.h>
#include <moaicore/MOAIGlyphCachePage.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIGlyph.h>
#include <moaicore/MOAIImage.h>
#include <moaicore/MOAIImageTexture.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAITextureBase.h>

//================================================================//
// MOAIGlyphCache
//================================================================//

//----------------------------------------------------------------//
void MOAIGlyphCache::ClearPages () {

	for ( u32 i = 0; i < this->mPages.Size (); ++i ) {
		delete this->mPages [ i ];
	}
	this->mPages.Clear ();
}

//----------------------------------------------------------------//
MOAIImage* MOAIGlyphCache::GetGlyphImage ( MOAIGlyph& glyph ) {

	assert ( glyph.GetPageID () < this->mPages.Size ());
	return this->mPages [ glyph.GetPageID ()]->mImageTexture;
}

//----------------------------------------------------------------//
MOAITextureBase* MOAIGlyphCache::GetGlyphTexture ( MOAIGlyph& glyph ) {

	assert ( glyph.GetPageID () < this->mPages.Size ());
	return this->mPages [ glyph.GetPageID ()]->mImageTexture;
}

//----------------------------------------------------------------//
MOAIImage* MOAIGlyphCache::GetImage () {

	u32 totalPages = this->mPages.Size ();
	if ( !totalPages ) return 0;

	u32 width = 0;
	u32 height = 0;

	for ( u32 i = 0; i < totalPages; ++i ) {
		MOAIImage& srcImage = *this->mPages [ i ]->mImageTexture;
		
		width = srcImage.GetWidth ();
		height += srcImage.GetHeight ();
	}
	
	MOAIImage& srcImage0 = *this->mPages [ 0 ]->mImageTexture;
	MOAIImage* image = new MOAIImage ();
	
	image->Init (
		width,
		height,
		srcImage0.GetColorFormat (),
		srcImage0.GetPixelFormat ()
	);
	
	u32 y = 0;
	for ( u32 i = 0; i < totalPages; ++i ) {
		MOAIImage& srcImage = *this->mPages [ i ]->mImageTexture;
		
		u32 copyHeight = srcImage.GetHeight ();
		image->CopyBits ( srcImage, 0, 0, 0, y, width, copyHeight );
		y += copyHeight;
	}
	
	return image;
}

//----------------------------------------------------------------//
bool MOAIGlyphCache::IsDynamic () {

	return true;
}

//----------------------------------------------------------------//
MOAIGlyphCache::MOAIGlyphCache () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGlyphCacheBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGlyphCache::~MOAIGlyphCache () {

	this->ClearPages ();
}

//----------------------------------------------------------------//
void MOAIGlyphCache::PlaceGlyph ( MOAIFont& font, MOAIGlyph& glyph ) {

	for ( u32 i = 0; i < this->mPages.Size (); ++i ) {
		MOAIGlyphCachePage* page = this->mPages [ i ];
		MOAISpan < MOAIGlyph* >* span = page->Alloc ( font, glyph );
		if ( span ) {
			this->mPages [ i ]->mImageTexture->Invalidate ();
			glyph.SetPageID ( i );
			return;
		}
	}
	
	u32 pageID = this->mPages.Size ();
	this->mPages.Resize ( pageID + 1 );
	
	MOAIGlyphCachePage* page = new MOAIGlyphCachePage ();
	this->mPages [ pageID ] = page;
	page->mColorFormat = this->mColorFormat;

	page->Alloc ( font, glyph );
	glyph.SetPageID ( pageID );
}

//----------------------------------------------------------------//
void MOAIGlyphCache::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIGlyphCacheBase::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIGlyphCache::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIGlyphCacheBase::RegisterLuaFuncs ( state );
}

//----------------------------------------------------------------//
void MOAIGlyphCache::RemoveGlyph ( MOAIGlyph& glyph ) {
	UNUSED ( glyph );
}

//----------------------------------------------------------------//
void MOAIGlyphCache::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIGlyphCache::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIGlyphCache::SetImage ( MOAIFont& font, MOAIImage& image ) {
	UNUSED ( font );
	UNUSED ( image );
}
