// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGLYPHCACHE_H
#define	MOAIGLYPHCACHE_H

#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIGlyphCachePage.h>
#include <moaicore/MOAIGlyphDeck.h>
#include <moaicore/MOAIImageTexture.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAISpanList.h>

//================================================================//
// MOAIGlyphCache
//================================================================//
class MOAIGlyphCache :
	public MOAILuaObject {
protected:

	friend class MOAIFont;

	static const u32 BAD_KEY = 0xffffffff;

	MOAIFont* mFont;
	USLeanArray < MOAIGlyphCachePage* > mPages;

	//----------------------------------------------------------------//
	static int		_defrag					( lua_State* L );
	static int		_getImage				( lua_State* L );
	static int		_setImage				( lua_State* L );

	//----------------------------------------------------------------//
	void			ClearPages				();
	
public:
	
	DECL_LUA_FACTORY ( MOAIGlyphCache )
	
	//----------------------------------------------------------------//
	MOAIImage*		GetGlyphImage			( MOAIGlyph& glyph );
	MOAIImage*		GetImage				();
					MOAIGlyphCache			();
					~MOAIGlyphCache			();
	void			PlaceGlyph				( MOAIGlyph& glyph );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			RemoveGlyph				( MOAIGlyph& glyph );
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void			SetImage				( MOAIImage& image );
	
	//----------------------------------------------------------------//
	void MOAIGlyphCache::DrawGlyph ( const MOAIGlyph& glyph, float x, float y ) {

		glyph.Draw ( *this->mPages [ glyph.mCacheKey ]->mImageTexture, x, y );
	}
};

#endif
