// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFREETYPEFONTBUILDER_H
#define	MOAIFREETYPEFONTBUILDER_H

#if USE_FREETYPE

#include <contrib/utf8.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H

#include <moaicore/MOAIFontBuilder.h>

//================================================================//
// MOAIFreeTypeFontBuilder
//================================================================//
class MOAIFreeTypeFontBuilder :
	public MOAIFontBuilder {
private:

	static const u32 MAX_KERN_TABLE_SIZE = 256;

	bool			mBuildKerningTables;

	//----------------------------------------------------------------//
	static int		_getBuildKerningTables		( lua_State* L );
	static int		_setBuildKerningTables		( lua_State* L );

	//----------------------------------------------------------------//
	void			BuildKerning				( FT_Face face, MOAIGlyph* glyphs, MOAIGlyph* pendingGlyphs );
	void			ProcessGlyphs				( FT_Face face, MOAIGlyphDeck& glyphDeck );
	void			RebuildKerning				( FT_Face face, MOAIGlyphDeck& glyphDeck );

public:
	
	DECL_LUA_FACTORY ( MOAIFreeTypeFontBuilder )
	
	//----------------------------------------------------------------//
					MOAIFreeTypeFontBuilder		();
					~MOAIFreeTypeFontBuilder	();
	void			ProcessGlyphs				();
	void			ProcessGlyphs				( MOAIFont& font );
	void			RebuildKerning				( MOAIFont& font );
	void			RebuildKerning				( MOAIFont& font, float points );
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
	void			SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
#endif
