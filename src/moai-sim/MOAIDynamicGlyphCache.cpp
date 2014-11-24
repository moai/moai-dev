// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/moai_utf8.h>
#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAIDynamicGlyphCache.h>
#include <moai-sim/MOAIDynamicGlyphCachePage.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGlyph.h>
#include <moai-sim/MOAIImage.h>
#include <moai-sim/MOAIImageTexture.h>
#include <moai-sim/MOAITextureBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	setColorFormat
	@text	The color format may be used by dynamic cache implementations
			when allocating new textures.
	
	@in		MOAIDynamicGlyphCache self
	@in		number colorFmt		One of MOAIImage.COLOR_FMT_A_8, MOAIImage.COLOR_FMT_RGB_888, MOAIImage.COLOR_FMT_RGB_565,
								MOAIImage.COLOR_FMT_RGBA_5551, MOAIImage.COLOR_FMT_RGBA_4444, COLOR_FMT_RGBA_8888
	@out	nil
*/
int MOAIDynamicGlyphCache::_setColorFormat ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDynamicGlyphCache, "UN" )

	self->mColorFormat = ( ZLColor::ColorFormat )state.GetValue < u32 >( 2, ( u32 )ZLColor::A_8 );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDynamicGlyphCache::_setPadding ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDynamicGlyphCache, "U" )
	
	if ( state.CheckParams ( 2, "NNNN", false )) {
	
		self->mPadding.mXMin = state.GetValue < float >( 2, 0.0f );
		self->mPadding.mYMin = state.GetValue < float >( 3, 0.0f );
		
		self->mPadding.mXMax = state.GetValue < float >( 4, 0.0f );
		self->mPadding.mYMax = state.GetValue < float >( 5, 0.0f );
	}
	else {
	
		float hPad = state.GetValue < float >( 2, 0.0f );
		float vPad = state.GetValue < float >( 3, hPad );
	
		hPad *= 0.5f;
		vPad *= 0.5f;
		
		self->mPadding.mXMin = -hPad;
		self->mPadding.mYMin = -vPad;
		
		self->mPadding.mXMax = hPad;
		self->mPadding.mYMax = vPad;
	}
	return 0;
}

//================================================================//
// MOAIDynamicGlyphCache
//================================================================//

//----------------------------------------------------------------//
void MOAIDynamicGlyphCache::ClearPages () {

	for ( u32 i = 0; i < this->mPages.Size (); ++i ) {
		this->mPages [ i ]->Clear ( *this );
		delete this->mPages [ i ];
	}
	this->mPages.Clear ();
}

//----------------------------------------------------------------//
MOAIImage* MOAIDynamicGlyphCache::GetGlyphImage ( MOAIGlyph& glyph ) {

	assert ( glyph.GetPageID () < this->mPages.Size ());
	return this->mPages [ glyph.GetPageID ()]->mImageTexture;
}

//----------------------------------------------------------------//
MOAITextureBase* MOAIDynamicGlyphCache::GetGlyphTexture ( MOAIGlyph& glyph ) {

	assert ( glyph.GetPageID () < this->mPages.Size ());
	return this->mPages [ glyph.GetPageID ()]->mImageTexture;
}

//----------------------------------------------------------------//
MOAIImage* MOAIDynamicGlyphCache::GetImage () {

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
		image->Blit ( srcImage, 0, 0, 0, y, width, copyHeight );
		y += copyHeight;
	}
	
	return image;
}

//----------------------------------------------------------------//
bool MOAIDynamicGlyphCache::IsDynamic () {

	return true;
}

//----------------------------------------------------------------//
MOAIDynamicGlyphCache::MOAIDynamicGlyphCache () :
	mColorFormat ( ZLColor::A_8 ) {
	
	this->mPadding.Init ( -1.0f, -1.0f, 1.0f, 1.0f );
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGlyphCache )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIDynamicGlyphCache::~MOAIDynamicGlyphCache () {

	this->ClearPages ();
}

//----------------------------------------------------------------//
int MOAIDynamicGlyphCache::PlaceGlyph ( MOAIFont& font, MOAIGlyph& glyph ) {

	for ( u32 i = 0; i < this->mPages.Size (); ++i ) {
		MOAIDynamicGlyphCachePage* page = this->mPages [ i ];
		MOAISpan < MOAIGlyph* >* span = page->Alloc ( *this, font, glyph );
		if ( span ) {
			this->mPages [ i ]->mImageTexture->UpdateRegion ();
			glyph.SetPageID ( i );
			return STATUS_OK;
		}
	}
	
	u32 pageID = this->mPages.Size ();
	this->mPages.Resize ( pageID + 1 );
	
	MOAIDynamicGlyphCachePage* page = new MOAIDynamicGlyphCachePage ();
	this->mPages [ pageID ] = page;

	page->Alloc ( *this, font, glyph );
	glyph.SetPageID ( pageID );
	
	page->mImageTexture->UpdateRegion ();
	
	return STATUS_OK;
}

//----------------------------------------------------------------//
void MOAIDynamicGlyphCache::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIGlyphCache::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIDynamicGlyphCache::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIGlyphCache::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "setColorFormat",			_setColorFormat },
		{ "setPadding",				_setPadding },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIDynamicGlyphCache::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIDynamicGlyphCache::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}
