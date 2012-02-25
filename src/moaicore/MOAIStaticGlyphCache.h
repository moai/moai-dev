// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISTATICGLYPHCACHE_H
#define	MOAISTATICGLYPHCACHE_H

#include <moaicore/MOAIGlyphCacheBase.h>

class MOAITexture;

//================================================================//
// MOAIStaticGlyphCache
//================================================================//
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
	bool				IsDynamic					();
						MOAIStaticGlyphCache		();
						~MOAIStaticGlyphCache		();
	void				PlaceGlyph					( MOAIGlyph& glyph );
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				RemoveGlyph					( MOAIGlyph& glyph );
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
	void				SetImage					( MOAIImage& image );
};

#endif
