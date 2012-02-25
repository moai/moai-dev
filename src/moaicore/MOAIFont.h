// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFONT_H
#define	MOAIFONT_H

#include <moaicore/MOAIGlyphSet.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAISpanList.h>

class MOAIFontReader;
class MOAIGlyph;
class MOAIGlyphCacheBase;
class MOAITextureBase;

#define DPI 72
#define POINTS_TO_PIXELS(points,dpi) (( points * dpi ) / DPI )
#define PIXELS_TO_POINTS(pixels,dpi) (( pixels * DPI ) / dpi )

//================================================================//
// MOAIFont
//================================================================//
class MOAIFont :
	public MOAILuaObject {
protected:

	friend class MOAIFreeTypeFontReader;

	STLString mFilename;
	u32 mFlags;
	
	MOAILuaSharedPtr < MOAIFontReader > mFontReader;
	MOAILuaSharedPtr < MOAIGlyphCacheBase > mGlyphCache;
	
	// for now
	typedef STLMap < float, MOAIGlyphSet >::iterator GlyphDecksIt;
	STLMap < float, MOAIGlyphSet > mGlyphDecks;

	//----------------------------------------------------------------//
	static int			_getFlags				( lua_State* L );
	static int			_load					( lua_State* L );
	static int			_preloadGlyphs			( lua_State* L );
	static int			_rebuildKerningTables	( lua_State* L );
	static int			_setFlags				( lua_State* L );
	static int			_setFontReader			( lua_State* L );
	static int			_setGlyphCache			( lua_State* L );

	//----------------------------------------------------------------//
	void				BuildKerning			( MOAIGlyph* glyphs, MOAIGlyph* pendingGlyphs );
	void				RebuildKerning			( MOAIGlyphSet& glyphDeck );

public:
	
	DECL_LUA_FACTORY ( MOAIFont )
	
	GET ( cc8*, Filename, mFilename );
	GET ( MOAIGlyphCacheBase*, GlyphCache, mGlyphCache );
	
	enum {
		FONT_AUTOLOAD_KERNING		= 0x01,
	};
	
	static const u32 DEFAULT_FLAGS = FONT_AUTOLOAD_KERNING;
	
	//----------------------------------------------------------------//
	void				AffirmGlyph				( float points, u32 c );
	MOAIGlyphSet*		GetGlyphDeck			( float points );
	MOAITextureBase*	GetGlyphTexture			( MOAIGlyph& glyph );
	void				Init					( cc8* filename );
	static bool			IsWhitespace			( u32 c );
						MOAIFont				();
						~MOAIFont				();
	void				ProcessGlyphs			();
	void				RebuildKerning			();
	void				RebuildKerning			( float points );
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				ResizePageList			( u32 size );
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
