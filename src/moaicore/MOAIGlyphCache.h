// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGLYPHCACHE_H
#define	MOAIGLYPHCACHE_H

#include <moaicore/MOAIGlyphCacheBase.h>

class MOAIFont;
class MOAIGlyphCachePage;

//================================================================//
// MOAIGlyphCache
//================================================================//
// TODO: doxygen
class MOAIGlyphCache :
	public MOAIGlyphCacheBase {
protected:

	MOAIFont* mFont;
	USLeanArray < MOAIGlyphCachePage* > mPages;

	//----------------------------------------------------------------//
	static int			_defrag						( lua_State* L );

	//----------------------------------------------------------------//
	void				ClearPages					();
	
public:
	
	DECL_LUA_FACTORY ( MOAIGlyphCache )
	
	//----------------------------------------------------------------//
	MOAIImage*			GetGlyphImage				( MOAIGlyph& glyph );
	MOAITextureBase*	GetGlyphTexture				( MOAIGlyph& glyph );
	MOAIImage*			GetImage					();
	bool				IsDynamic					();
						MOAIGlyphCache				();
						~MOAIGlyphCache				();
	void				PlaceGlyph					( MOAIFont& font, MOAIGlyph& glyph );
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				RemoveGlyph					( MOAIGlyph& glyph );
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
	void				SetImage					( MOAIFont& font, MOAIImage& image );
};

#endif
