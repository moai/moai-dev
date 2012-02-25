// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFONT_H
#define	MOAIFONT_H

#include <moaicore/MOAIGlyphDeck.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAISpanList.h>

class MOAIFontBuilder;
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

	friend class MOAIFreeTypeFontBuilder;

	STLString mFilename;
	
	MOAILuaSharedPtr < MOAIFontBuilder > mFontBuilder;
	MOAILuaSharedPtr < MOAIGlyphCacheBase > mGlyphCache;
	
	// for now
	typedef STLMap < float, MOAIGlyphDeck >::iterator GlyphDecksIt;
	STLMap < float, MOAIGlyphDeck > mGlyphDecks;

	//----------------------------------------------------------------//
	static int			_load					( lua_State* L );
	static int			_preloadGlyphs			( lua_State* L );
	static int			_rebuildKerningTables	( lua_State* L );
	static int			_setFontBuilder			( lua_State* L );
	static int			_setGlyphCache			( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIFont )
	
	GET ( cc8*, Filename, mFilename );
	
	//----------------------------------------------------------------//
	void				AffirmGlyph				( float points, u32 c );
	MOAIGlyphDeck*		GetGlyphDeck			( float points );
	MOAITextureBase*	GetGlyphTexture			( MOAIGlyph& glyph );
	void				Init					( cc8* filename );
	static bool			IsWhitespace			( u32 c );
						MOAIFont				();
						~MOAIFont				();
	void				ProcessGlyphs			();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				ResizePageList			( u32 size );
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
