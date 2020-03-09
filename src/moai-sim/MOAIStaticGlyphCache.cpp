// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/moai_utf8.h>
#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAIGlyph.h>
#include <moai-sim/MOAIStaticGlyphCache.h>

//================================================================//
// MOAIStaticGlyphCache
//================================================================//

//----------------------------------------------------------------//
void MOAIStaticGlyphCache::ClearTextures () {

	for ( ZLIndex i = 0; i < this->mTextures.Size (); ++i ) {
		this->LuaRelease ( this->mTextures [ i ]); // TODO: ref counting?
	}
	this->mTextures.Clear ();
}

//----------------------------------------------------------------//
MOAIImage* MOAIStaticGlyphCache::GetGlyphImage ( MOAIGlyph& glyph ) {
	UNUSED ( glyph );
	return 0;
}

//----------------------------------------------------------------//
MOAITexture* MOAIStaticGlyphCache::GetGlyphTexture ( MOAIGlyph& glyph ) {

	assert ( glyph.GetPageID () < this->mTextures.Size ());
	return this->mTextures [ glyph.GetPageID ()];
}

//----------------------------------------------------------------//
MOAIImage* MOAIStaticGlyphCache::GetImage () {

	return 0;
}

//----------------------------------------------------------------//
MOAITexture* MOAIStaticGlyphCache::GetTexture ( ZLIndex id ) {

	return this->mTextures [ id ];
}

//----------------------------------------------------------------//
bool MOAIStaticGlyphCache::IsDynamic () {

	return false;
}

//----------------------------------------------------------------//
MOAIStaticGlyphCache::MOAIStaticGlyphCache () {
	
	MOAI_LUA_OBJECT_RTTI_BEGIN ( MOAIStaticGlyphCache )
		RTTI_EXTEND ( MOAIGlyphCache )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIStaticGlyphCache::~MOAIStaticGlyphCache () {

	this->ClearTextures ();
}

//----------------------------------------------------------------//
void MOAIStaticGlyphCache::ReserveTextures ( u32 total ) {

	this->mTextures.Init ( total );
	this->mTextures.Fill ( 0 );
}

//----------------------------------------------------------------//
int MOAIStaticGlyphCache::SetImage ( MOAIFont& font, MOAIImage& image ) {

	this->ClearTextures ();

	u32 width = image.GetWidth ();
	u32 height = image.GetHeight ();

	if ( !( width && height )) return STATUS_ERROR;

	u32 totalTextures = ( height / width ) + 1;
	this->mTextures.Init ( totalTextures );
	
	u32 y = 0;
	for ( ZLIndex i = 0; i < totalTextures; ++i ) {

		// TODO: Gfx

//		MOAITexture* texture = new MOAITexture ();
//
//		u32 textureHeight = height - y;
//		textureHeight = textureHeight > width ? width : textureHeight;
//
//		texture->Init ( image, 0, y, width, textureHeight, font.GetFilename ());
//		texture->SetFilter ( font.GetMinFilter (), font.GetMagFilter ());
//
//		y += textureHeight;
//
//		this->SetTexture ( i, texture );
	}
	return STATUS_OK;
}

//----------------------------------------------------------------//
void MOAIStaticGlyphCache::SetTexture ( ZLIndex id, MOAITexture* texture ) {
	this->LuaRetain ( texture ); // TODO: ref counting?
	this->mTextures [ id ] = texture;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIStaticGlyphCache::MOAILuaObject_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIStaticGlyphCache::MOAILuaObject_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIStaticGlyphCache::MOAILuaObject_SerializeIn ( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIStaticGlyphCache::MOAILuaObject_SerializeOut ( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
	if ( history.DidVisit ( *this )) return;
}
