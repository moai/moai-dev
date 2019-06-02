// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISTATICGLYPHCACHE_H
#define	MOAISTATICGLYPHCACHE_H

#include <moai-sim/MOAIGlyphCache.h>

class MOAITexture;

//================================================================//
// MOAIStaticGlyphCache
//================================================================//
/**	@lua	MOAIStaticGlyphCache
	@text	This is the default implementation of a static glyph cache.
			All is does is accept an image via setImage () and create
			a set of textures from that image. It does not implement
			getImage ().
*/
class MOAIStaticGlyphCache :
	public MOAIGlyphCache {
protected:

	ZLLeanArray < MOAITexture* > mTextures;

	//----------------------------------------------------------------//
	void				ClearTextures				();
	
public:
	
	DECL_LUA_FACTORY ( MOAIStaticGlyphCache )
	
	//----------------------------------------------------------------//
	MOAIImage*			GetGlyphImage				( MOAIGlyph& glyph );
	MOAITexture*	GetGlyphTexture				( MOAIGlyph& glyph );
	MOAIImage*			GetImage					();
	MOAITexture*		GetTexture					( ZLIndex id );
	bool				IsDynamic					();
						MOAIStaticGlyphCache		();
						~MOAIStaticGlyphCache		();
	void				MOAILuaObject_RegisterLuaClass			( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs			( MOAIComposer& composer, MOAILuaState& state );
	void				ReserveTextures				( u32 total );
	void				MOAILuaObject_SerializeIn					( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer );
	void				MOAILuaObject_SerializeOut				( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer );
	int					SetImage					( MOAIFont& font, MOAIImage& image );
	void				SetTexture					( ZLIndex id, MOAITexture* texture );
};

#endif
