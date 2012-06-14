// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISTATICGLYPHCACHE_H
#define	MOAISTATICGLYPHCACHE_H

#include <moaicore/MOAIGlyphCacheBase.h>

class MOAITexture;

//================================================================//
// MOAIStaticGlyphCache
//================================================================//
/**	@name	MOAIStaticGlyphCache
	@text	This is the default implementation of a static glyph cache.
			All is does is accept an image via setImage () and create
			a set of textures from that image. It does not implement
			getImage ().
*/
class MOAIStaticGlyphCache :
	public MOAIGlyphCacheBase {
protected:

	USLeanArray < MOAITexture* > mTextures;

	//----------------------------------------------------------------//
	void				ClearTextures				();
	
public:
	
	DECL_LUA_FACTORY ( MOAIStaticGlyphCache )
	
	//----------------------------------------------------------------//
	MOAIImage*			GetGlyphImage				( MOAIGlyph& glyph );
	MOAITextureBase*	GetGlyphTexture				( MOAIGlyph& glyph );
	MOAIImage*			GetImage					();
	MOAITexture*		GetTexture					( u32 id );
	bool				IsDynamic					();
						MOAIStaticGlyphCache		();
						~MOAIStaticGlyphCache		();
	void				PlaceGlyph					( MOAIFont& font, MOAIGlyph& glyph );
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				RemoveGlyph					( MOAIGlyph& glyph );
	void				ReserveTextures				( u32 total );
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
	void				SetImage					( MOAIFont& font, MOAIImage& image );
	void				SetTexture					( int id, MOAITexture * texture );
};

#endif
