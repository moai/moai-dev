// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/moai_utf8.h>
#include <moai-sim/MOAIGlyphCache.h>
#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAIImage.h>

//================================================================//
// MOAIGlyphCache
//================================================================//

//----------------------------------------------------------------//
MOAIGlyphCache::MOAIGlyphCache () {
	
	RTTI_BEGIN
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
void MOAIGlyphCache::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIGlyphCache::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
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
