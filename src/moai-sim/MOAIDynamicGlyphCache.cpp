// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/moai_utf8.h>
#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAIDynamicGlyphCache.h>
#include <moai-sim/MOAIDynamicGlyphCachePage.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGlyph.h>
#include <moai-sim/MOAIImage.h>
#include <moai-sim/MOAIImageTexture.h>
#include <moai-sim/MOAITextureBase.h>

#ifdef MOAIDYNAMICGLYPHCACHE_DEBUG
	#define DEBUG_LOG printf
#else
	#define DEBUG_LOG(...)
#endif

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
/**	@lua	setPadding
	@text	Add padding to glyphs when ripping from font.
	
	@overload
	
		@in		MOAIDynamicGlyphCache self
		@in		number hPad		glyph xMin -= hPad * 0.5, glyph xMax += hPad * 0.5
		@in		number vPad		glyph yMin -= vPad * 0.5, glyph yMax += vPad * 0.5
		@out	nil

	@overload
	
		@in		MOAIDynamicGlyphCache self
		@in		xMinP			glyph xMin -= xMinP
		@in		yMinP			glyph yMin -= yMinP
		@in		xMaxP			glyph xMax += xMaxP
		@in		yMaxP			glyph yMax += yMaxP
		@out	nil
*/
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

	size_t totalPages = this->mPages.Size ();
	if ( !totalPages ) return 0;

	u32 width = 0;
	u32 height = 0;

	for ( size_t i = 0; i < totalPages; ++i ) {
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
	for ( size_t i = 0; i < totalPages; ++i ) {
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

	DEBUG_LOG ( "PLACE GLYPH %d: (%d x %d) - %s\n",
		glyph.GetCode (),
		( u32 )( glyph.mWidth + this->mPadding.Width ()),
		( u32 )( glyph.mHeight + this->mPadding.Height ()),
		font.GetFilename ()
	);

	for ( u32 i = 0; i < this->mPages.Size (); ++i ) {
		DEBUG_LOG ( "  TRYING PAGE: %d\n", i );
		MOAIDynamicGlyphCachePage* page = this->mPages [ i ];
		MOAISpan < MOAIGlyph* >* span = page->Alloc ( *this, font, glyph );
		if ( span ) {
			DEBUG_LOG ( "  PLACED IN PAGE: %d\n", i );
			this->mPages [ i ]->mImageTexture->UpdateRegion ();
			glyph.SetPageID ( i );
			return STATUS_OK;
		}
	}
	
	u32 pageID = ( u32 )this->mPages.Size (); // TODO: cast
	this->mPages.Resize ( pageID + 1 );
	
	DEBUG_LOG ( "  NEW PAGE: %d\n", pageID );
	
	MOAIDynamicGlyphCachePage* page = new MOAIDynamicGlyphCachePage ();
	page->SetPageID ( pageID );
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
