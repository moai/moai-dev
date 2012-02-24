// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFONT_H
#define	MOAIFONT_H

#include <moaicore/MOAIGlyphCache.h>
#include <moaicore/MOAIGlyphDeck.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAISpanList.h>

class MOAIGlyph;
class MOAIGlyphCache;
class MOAIImage;
class MOAIImageTexture;

#define DPI 72
#define POINTS_TO_PIXELS(points,dpi) (( points * dpi ) / DPI )
#define PIXELS_TO_POINTS(pixels,dpi) (( pixels * DPI ) / dpi )

//================================================================//
// MOAIFont
//================================================================//
class MOAIFont :
	public MOAILuaObject {
protected:

	static const u32 MAX_KERN_TABLE_SIZE = 256;

	STLString			mFilename;
	bool				mBuildKerningTables;
	MOAIGlyphCache*		mGlyphCache;
	
	// for now
	typedef STLMap < float, MOAIGlyphDeck >::iterator GlyphDecksIt;
	STLMap < float, MOAIGlyphDeck > mGlyphDecks;

	//----------------------------------------------------------------//
	static int			_getBuildKerningTables	( lua_State* L );
	static int			_load					( lua_State* L );
	static int			_preloadGlyphs			( lua_State* L );
	static int			_rebuildKerningTables	( lua_State* L );
	static int			_setBuildKerningTables	( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIFont )
	
	//----------------------------------------------------------------//
	void				AffirmGlyph				( float points, u32 c );
	MOAIGlyphDeck*		GetGlyphDeck			( float points );
	MOAIImageTexture*	GetPage					( u32 id );
	void				Init					( cc8* filename );
	static bool			IsWhitespace			( u32 c );
						MOAIFont				();
						~MOAIFont				();
	virtual void		ProcessGlyphs			();
	virtual void		RebuildKerning			();
	virtual void		RebuildKerning			( float points );
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				ResizePageList			( u32 size );
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	
	//----------------------------------------------------------------//
	inline void DrawGlyph ( const MOAIGlyph& glyph, float x, float y ) {

		assert ( this->mGlyphCache );
		this->mGlyphCache->DrawGlyph ( glyph, x, y );
	}
};

#endif
