// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFREETYPEFONT_H
#define	MOAIFREETYPEFONT_H

#if USE_FREETYPE

#include <contrib/utf8.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H

#include <moaicore/MOAIFont.h>

//================================================================//
// MOAIFreeTypeFont
//================================================================//
class MOAIFreeTypeFont :
	public MOAIFont {
private:

	//----------------------------------------------------------------//
	void				BuildKerning			( FT_Face face, MOAIGlyph* glyphs, MOAIGlyph* pendingGlyphs );
	void				ProcessGlyphs			( FT_Face face, MOAIGlyphDeck& glyphDeck );
	void				RebuildKerning			( FT_Face face, MOAIGlyphDeck& glyphDeck );

public:
	
	DECL_LUA_FACTORY ( MOAIFreeTypeFont )
	
	//----------------------------------------------------------------//
						MOAIFreeTypeFont		();
						~MOAIFreeTypeFont		();
	void				ProcessGlyphs			();
	void				RebuildKerning			();
	void				RebuildKerning			( float points );
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
#endif
