// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/moai_utf8.h>
#include <moai-sim/MOAIGlyphCache.h>
#include <moai-sim/MOAIFont.h>

//================================================================//
// MOAIGlyphCache
//================================================================//

//----------------------------------------------------------------//
MOAIGlyphCache::MOAIGlyphCache ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ) {
	
	RTTI_BEGIN ( MOAIGlyphCache )
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGlyphCache::~MOAIGlyphCache () {
}

//----------------------------------------------------------------//
int MOAIGlyphCache::PlaceGlyph ( MOAIFont& font, MOAIGlyph& glyph ) {
	UNUSED ( font );
	UNUSED ( glyph );
	
	return STATUS_UNSUPPORTED;
}

//----------------------------------------------------------------//
void MOAIGlyphCache::PostRender ( MOAIGlyph& glyph ) {
	UNUSED ( glyph );
}

//----------------------------------------------------------------//
int MOAIGlyphCache::RemoveGlyph ( MOAIGlyph& glyph ) {
	UNUSED ( glyph );
	
	return STATUS_UNSUPPORTED;
}

//----------------------------------------------------------------//
int MOAIGlyphCache::SetImage ( MOAIFont& font, MOAIImage& image ) {
	UNUSED ( font );
	UNUSED ( image );
	
	return STATUS_UNSUPPORTED;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
