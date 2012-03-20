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
// TODO: doxygen
class MOAIFont :
	public MOAILuaObject {
protected:

	STLString mFilename;
	u32 mFlags;
	
	MOAILuaSharedPtr < MOAIFontReader > mReader;
	MOAILuaSharedPtr < MOAIGlyphCacheBase > mCache;
	
	// for now
	typedef STLMap < float, MOAIGlyphSet >::iterator GlyphSetsIt;
	STLMap < float, MOAIGlyphSet > mGlyphSets;

	//----------------------------------------------------------------//
	static int			_getFlags				( lua_State* L );
	static int			_getImage				( lua_State* L );
	static int			_load					( lua_State* L );
	static int			_preloadGlyphs			( lua_State* L );
	static int			_rebuildKerningTables	( lua_State* L );
	static int			_setCache				( lua_State* L );
	static int			_setFlags				( lua_State* L );
	static int			_setImage				( lua_State* L );
	static int			_setReader				( lua_State* L );

	//----------------------------------------------------------------//
	void				BuildKerning			( MOAIGlyph* glyphs, MOAIGlyph* pendingGlyphs );
	void				RebuildKerning			( MOAIGlyphSet& glyphSet );

public:
	
	DECL_LUA_FACTORY ( MOAIFont )
	
	GET ( cc8*, Filename, mFilename );
	GET ( MOAIGlyphCacheBase*, Cache, mCache );
	
	enum {
		FONT_AUTOLOAD_KERNING		= 0x01,
	};
	
	static const u32 DEFAULT_FLAGS = FONT_AUTOLOAD_KERNING;
	
	//----------------------------------------------------------------//
	void				AffirmGlyph				( float points, u32 c );
	MOAIGlyphSet*		GetGlyphDeck			( float points );
	MOAITextureBase*	GetGlyphTexture			( MOAIGlyph& glyph );
	void				Init					( cc8* filename );
	static bool			IsControl				( u32 c );
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
